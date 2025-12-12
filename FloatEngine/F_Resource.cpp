#include "F_Resource.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <stdexcept>

#include "FloatApi.h" 

// Minizip-ng includes
#include "minizip-ng/mz.h"
#include "minizip-ng/mz_zip.h"
#include "minizip-ng/mz_strm.h"
#include "minizip-ng/mz_strm_mem.h"
#include "minizip-ng/mz_zip_rw.h"

// ----------------------------------------------------------------------------
// 构造函数 / 析构函数 / 配置 Setters
// ----------------------------------------------------------------------------

F_Resource::F_Resource() : use_memory_zip(false) {}
F_Resource::~F_Resource() { UnloadAllResources(); }

void F_Resource::SetResourcePath(const std::string& path) { resource_path = path; use_memory_zip = false; resource_data.clear(); }
void F_Resource::SetResourceData(const std::vector<unsigned char>& data) { resource_data = data; use_memory_zip = true; resource_path.clear(); }
void F_Resource::SetPassword(const std::string& password) { zip_password = password; }
void F_Resource::SetMemoryBudget(size_t maxBytes) { m_maxMemoryBudget = maxBytes; }
void F_Resource::SetEnableMemoryMonitoring(bool enable) { m_enableMemoryMonitoring = enable; }

// ----------------------------------------------------------------------------
// Minizip-ng 核心：ExtractFileFromZip (I/O)
// ----------------------------------------------------------------------------
std::vector<unsigned char> F_Resource::ExtractFileFromZip(const std::string& internalPath) {
    std::vector<unsigned char> data;
    void* zip_reader = mz_zip_reader_create();

    if (!zip_reader) { FLOG_ERRORF("Failed to create ZIP reader"); return data; }
    if (!zip_password.empty()) mz_zip_reader_set_password(zip_reader, zip_password.c_str());

    int32_t err = MZ_OK;
    void* mem_stream = nullptr;

    if (use_memory_zip) {
        mem_stream = mz_stream_mem_create();
        if (!mem_stream) { mz_zip_reader_delete(&zip_reader); return data; }
        mz_stream_mem_set_buffer(mem_stream, (void*)resource_data.data(), (int32_t)resource_data.size());
        err = mz_zip_reader_open(zip_reader, mem_stream);
    }
    else {
        err = mz_zip_reader_open_file(zip_reader, resource_path.c_str());
    }

    if (err != MZ_OK) {
        FLOG_ERRORF("Failed to open ZIP source. Error: %d", err);
        if (mem_stream) mz_stream_mem_delete(&mem_stream);
        mz_zip_reader_delete(&zip_reader);
        return data;
    }

    err = mz_zip_reader_locate_entry(zip_reader, internalPath.c_str(), 1);

    if (err == MZ_OK) {
        mz_zip_file* file_info = nullptr;
        mz_zip_reader_entry_get_info(zip_reader, &file_info);

        if (file_info) {
            err = mz_zip_reader_entry_open(zip_reader);

            if (err == MZ_OK) {
                int64_t file_size = file_info->uncompressed_size;
                if (file_size > 0) {
                    data.resize((size_t)file_size);
                    int32_t bytes_read = mz_zip_reader_entry_read(zip_reader, data.data(), (int32_t)file_size);

                    if (bytes_read != file_size) {
                        FLOG_ERRORF("Error reading entry: expected %lld, got %d", file_size, bytes_read);
                        data.clear();
                    }
                }
                mz_zip_reader_entry_close(zip_reader);
            }
        }
    }

    mz_zip_reader_close(zip_reader);
    mz_zip_reader_delete(&zip_reader);
    if (mem_stream) mz_stream_mem_delete(&mem_stream);

    return data;
}

// ----------------------------------------------------------------------------
// 内存监控核心：LRU 和 Upload (GPU/Audio)
// ----------------------------------------------------------------------------
void F_Resource::CheckAndUnloadLRU(size_t requiredBytes) {
    if (!m_enableMemoryMonitoring) return;

    if (m_currentMemoryUsage.load() + requiredBytes <= m_maxMemoryBudget) return;

    std::vector<std::pair<std::string, std::chrono::steady_clock::time_point>> unloadCandidates;

    for (const auto& pair : resourceStates) {
        const std::string& id = pair.first;
        const ResourceState& state = pair.second;
        if (state.ref_count == 0) {
            unloadCandidates.push_back({ id, state.last_access_time });
        }
    }

    std::sort(unloadCandidates.begin(), unloadCandidates.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    size_t bytesToFree = (m_currentMemoryUsage.load() + requiredBytes) - m_maxMemoryBudget;
    size_t freedBytes = 0;

    for (const auto& candidate : unloadCandidates) {
        const std::string& id = candidate.first;
        if (resourceStates.count(id) && resourceStates[id].ref_count == 0) {
            size_t usage = resourceStates[id].memory_usage_bytes;
            ResourceType type = resourceStates[id].type;

            UnloadResource(id, type);

            freedBytes += usage;
            if (freedBytes >= bytesToFree) break;
        }
    }

    if (m_currentMemoryUsage.load() + requiredBytes > m_maxMemoryBudget) {
        FLOG_ERRORF("Memory allocation failed! Could not free enough memory for %zu bytes.", requiredBytes);
    }
}

bool F_Resource::UploadResourceToGPU(const LoadTask& task) {

    size_t requiredBytes = task.raw_data.size();
    if (task.type == ResTexture) { requiredBytes *= 4; } // 粗略估算 GPU 内存

    if (m_enableMemoryMonitoring) {
        CheckAndUnloadLRU(requiredBytes);
    }

    std::string ext = DetectFileFormat(task.raw_data);
    const std::string& id = task.id;
    bool success = false;

    switch (task.type) {
    case ResTexture: {
        Image img = LoadImageFromMemory(ext.c_str(), task.raw_data.data(), (int)task.raw_data.size());
        if (img.data != nullptr) { textures[id] = LoadTextureFromImage(img); UnloadImage(img); success = true; } break;
    }
    case ResSound: {
        Wave wave = LoadWaveFromMemory(ext.c_str(), task.raw_data.data(), (int)task.raw_data.size());
        if (wave.data != nullptr) { sounds[id] = LoadSoundFromWave(wave); UnloadWave(wave); success = true; } break;
    }
    case ResMusic: {
        std::string rawKey = id + "_internal_music_raw";
        datas[rawKey] = task.raw_data;
        musics[id] = LoadMusicStreamFromMemory(ext.c_str(), datas[rawKey].data(), (int)datas[rawKey].size());
        if (musics[id].stream.buffer != nullptr) success = true; else datas.erase(rawKey); break;
    }
    case ResFont: {
        int* cp_data = task.codepoints.empty() ? nullptr : const_cast<int*>(task.codepoints.data());
        int cp_size = (int)task.codepoints.size();
        fonts[id] = LoadFontFromMemory(ext.c_str(), task.raw_data.data(), (int)task.raw_data.size(), task.font_size, cp_data, cp_size);
        if (fonts[id].texture.id != 0) success = true; break;
    }
    case ResText: {
        texts[id] = std::string(reinterpret_cast<const char*>(task.raw_data.data()), task.raw_data.size());
        success = true; break;
    }
    case ResData: {
        datas[id] = task.raw_data;
        success = true; break;
    }
    default:
        break;
    }

    if (success && m_enableMemoryMonitoring) {
        m_currentMemoryUsage += requiredBytes;
        ResourceState state;
        state.ref_count = 0;
        state.memory_usage_bytes = requiredBytes;
        state.last_access_time = std::chrono::steady_clock::now();
        state.type = task.type;
        resourceStates[id] = state;
    }

    return success;
}

// ----------------------------------------------------------------------------
// 同步加载核心
// ----------------------------------------------------------------------------
bool F_Resource::LoadResourceInternal(const std::string& internalPath, const std::string& id, ResourceType type, int font_size, const std::vector<int>& codepoints) {

    // 1. 在当前线程执行 I/O (阻塞)
    std::vector<unsigned char> fileData = ExtractFileFromZip(internalPath);
    if (fileData.empty()) {
        FLOG_ERRORF("Failed to extract file data for %s.", internalPath.c_str());
        return false;
    }

    // 2. 构造 LoadTask
    LoadTask task;
    task.id = id;
    task.type = type;
    task.raw_data = std::move(fileData);
    task.font_size = font_size;
    task.codepoints = codepoints;

    // 3. 在当前线程执行 GPU 上传 (阻塞)
    return UploadResourceToGPU(task);
}

// ----------------------------------------------------------------------------
// 卸载/Getters/Helpers
// ----------------------------------------------------------------------------
bool F_Resource::LoadResource(const std::string& internalPath, const std::string& id, ResourceType type, bool lazyLoad) {
    if (HasResource(id, type)) return true;

    if (lazyLoad) {
        if (lazyLoadedResources.count(id)) return false;
        ResourceMetadata metadata = { type, internalPath, 0, {} };
        lazyLoadedResources[id] = metadata;
        return true;
    }
    else {
        return LoadResourceInternal(internalPath, id, type);
    }
}

bool F_Resource::LoadFontResource(const std::string& internalPath, const std::string& id, int font_size, const std::vector<int>& codepoints, bool lazyLoad) {
    if (HasResource(id, ResFont)) return true;

    if (lazyLoad) {
        if (lazyLoadedResources.count(id)) return false;

        ResourceMetadata metadata = { ResFont, internalPath, font_size, codepoints };
        lazyLoadedResources[id] = metadata;
        return true;
    }
    else {
        return LoadResourceInternal(internalPath, id, ResFont, font_size, codepoints);
    }
}

bool F_Resource::CreateLoadBatch(const std::string& batchId, const std::vector<std::pair<std::string, ResourceType>>& resources) {
    if (resourceBatches.count(batchId)) return false;
    std::map<std::string, ResourceMetadata> newBatch;
    for (const auto& pair : resources) {
		ResourceMetadata metadata = { pair.second, "", 0, {} };
        if (lazyLoadedResources.find(pair.first)!= lazyLoadedResources.end()) {
			metadata.internalPath = lazyLoadedResources[pair.first].internalPath;
			if (pair.second == ResFont) {
                metadata.font_size = lazyLoadedResources[pair.first].font_size;
                metadata.codepoints = lazyLoadedResources[pair.first].codepoints;
            }
        }
        newBatch[pair.first] = metadata;
    }
    resourceBatches[batchId] = std::move(newBatch);
    return true;
}

bool F_Resource::LoadResourceToBatch(const std::string& internalPath, const std::string& id, ResourceType type, const std::string& batchId) {
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end() || !batchIt->second.count(id)) return false;

    ResourceMetadata& metadata = batchIt->second.at(id);
    metadata.type = type;
    metadata.internalPath = internalPath;
    metadata.font_size = 0;
    metadata.codepoints.clear();

    return true;
}

bool F_Resource::LoadFontResourceToBatch(const std::string& internalPath, const std::string& id, int font_size, const std::string& batchId, const std::vector<int>& codepoints) {
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end() || !batchIt->second.count(id)) return false;

    ResourceMetadata& metadata = batchIt->second.at(id);
    metadata.type = ResFont;
    metadata.internalPath = internalPath;
    metadata.font_size = font_size;
    metadata.codepoints = codepoints;

    return true;
}

void F_Resource::LoadBatchResources(const std::string& batchId) {
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) return;

    for (const auto& pair : batchIt->second) {
        const std::string& resourceId = pair.first;
        const ResourceMetadata& metadata = pair.second;

        if (HasResource(resourceId, metadata.type)) continue;

        LoadResourceInternal(metadata.internalPath, resourceId, metadata.type, metadata.font_size, metadata.codepoints);
    }
}

void F_Resource::UnloadBatchResources(const std::string& batchId) {
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) return;

    for (const auto& pair : batchIt->second) {
        UnloadResource(pair.first, pair.second.type);
    }
}

// Getters (保持 LRU/引用计数逻辑)
Texture2D F_Resource::GetTexture(const std::string& id) {
    if (textures.count(id)) {
        if (m_enableMemoryMonitoring) { resourceStates[id].ref_count++; resourceStates[id].last_access_time = std::chrono::steady_clock::now(); }
        return textures[id];
    }
    return Texture2D{};
}

Sound F_Resource::GetSound(const std::string& id) {
    if (sounds.count(id)) {
        if (m_enableMemoryMonitoring) { resourceStates[id].ref_count++; resourceStates[id].last_access_time = std::chrono::steady_clock::now(); }
        return sounds[id];
    }
    return Sound{};
}

Music F_Resource::GetMusic(const std::string& id) {
    if (musics.count(id)) {
        if (m_enableMemoryMonitoring) { resourceStates[id].ref_count++; resourceStates[id].last_access_time = std::chrono::steady_clock::now(); }
        return musics[id];
    }
    return Music{};
}

Font F_Resource::GetFont(const std::string& id) {
    if (fonts.count(id)) {
        if (m_enableMemoryMonitoring) { resourceStates[id].ref_count++; resourceStates[id].last_access_time = std::chrono::steady_clock::now(); }
        return fonts[id];
    }
    return Font{};
}

std::string F_Resource::GetText(const std::string& id) {
    if (texts.count(id)) {
        if (m_enableMemoryMonitoring) { resourceStates[id].ref_count++; resourceStates[id].last_access_time = std::chrono::steady_clock::now(); }
        return texts[id];
    }
    return std::string();
}

std::vector<unsigned char> F_Resource::GetData(const std::string& id) {
    if (datas.count(id)) {
        if (m_enableMemoryMonitoring) { resourceStates[id].ref_count++; resourceStates[id].last_access_time = std::chrono::steady_clock::now(); }
        return datas[id];
    }
    return std::vector<unsigned char>();
}

// Helpers
bool F_Resource::HasResource(const std::string& id, ResourceType type) const {
    switch (type) {
    case ResTexture: return textures.count(id);
    case ResSound: return sounds.count(id);
    case ResMusic: return musics.count(id);
    case ResFont: return fonts.count(id);
    case ResText: return texts.count(id);
    case ResData: return datas.count(id);
    default: return false;
    }
}

ResourceType F_Resource::GetResourceType(const std::string& id) {
    if (textures.count(id)) return ResTexture;
    if (sounds.count(id))   return ResSound;
    if (musics.count(id))   return ResMusic;
    if (fonts.count(id))    return ResFont;
    if (texts.count(id))    return ResText;
    if (datas.count(id))    return ResData;

    auto lazyIt = lazyLoadedResources.find(id);
    if (lazyIt != lazyLoadedResources.end()) return lazyIt->second.type;

    for (const auto& batchPair : resourceBatches) {
        auto it = batchPair.second.find(id);
        if (it != batchPair.second.end()) return it->second.type;
    }
    return ResUnknown;
}

std::string F_Resource::GetResourcePath(const std::string& id) {
    auto lazyIt = lazyLoadedResources.find(id);
    if (lazyIt != lazyLoadedResources.end()) return lazyIt->second.internalPath;

    for (const auto& batchPair : resourceBatches) {
        auto it = batchPair.second.find(id);
        if (it != batchPair.second.end()) return it->second.internalPath;
    }
    return "";
}

int F_Resource::GetFileCount() {
    void* zip_reader = mz_zip_reader_create();
    if (!zip_reader) return 0;
    uint64_t count = 0;
    // (Minizip-ng GetFileCount 逻辑)
    if (use_memory_zip) {
        void* mem_stream = mz_stream_mem_create();
        mz_stream_mem_set_buffer(mem_stream, (void*)resource_data.data(), (int32_t)resource_data.size());
        if (mz_zip_reader_open(zip_reader, mem_stream) == MZ_OK) { mz_zip_get_number_entry(zip_reader, &count); }
        mz_zip_reader_close(zip_reader); mz_stream_mem_delete(&mem_stream);
    }
    else {
        if (mz_zip_reader_open_file(zip_reader, resource_path.c_str()) == MZ_OK) { mz_zip_get_number_entry(zip_reader, &count); }
        mz_zip_reader_close(zip_reader);
    }
    mz_zip_reader_delete(&zip_reader);
    return (int)count;
}

std::string F_Resource::DetectFileFormat(const std::vector<unsigned char>& data) {
    if (data.size() < 4) return ".dat";
    const unsigned char* bytes = data.data();
    if (bytes[0] == 0x89 && bytes[1] == 0x50 && bytes[2] == 0x4E && bytes[3] == 0x47) return ".png";
    // ... (其他魔数检测逻辑)
    if (bytes[0] == 0xFF && bytes[1] == 0xD8 && bytes[2] == 0xFF) return ".jpg";
    if (bytes[0] == 0x47 && bytes[1] == 0x49 && bytes[2] == 0x46) return ".gif";
    if (bytes[0] == 0x42 && bytes[1] == 0x4D) return ".bmp";
    if (bytes[0] == 0x52 && bytes[1] == 0x49 && bytes[2] == 0x46 && bytes[3] == 0x46) return ".wav";
    if (bytes[0] == 0x4F && bytes[1] == 0x67 && bytes[2] == 0x67 && bytes[3] == 0x53) return ".ogg";
    if ((bytes[0] == 0x49 && bytes[1] == 0x44 && bytes[2] == 0x33) || (bytes[0] == 0xFF && (bytes[1] & 0xE0) == 0xE0)) return ".mp3";
    if (bytes[0] == 0x00 && bytes[1] == 0x01 && bytes[2] == 0x00 && bytes[3] == 0x00) return ".ttf";
    if (bytes[0] == 0x77 && bytes[1] == 0x4F && bytes[2] == 0x46 && bytes[3] == 0x46) return ".woff";
    return ".dat";
}

bool F_Resource::IsValidZipFile() const {
    if (use_memory_zip) {
        if (resource_data.size() < 4) return false;
        return (resource_data[0] == 0x50 && resource_data[1] == 0x4B && resource_data[2] == 0x03 && resource_data[3] == 0x04);
    }
    else {
        std::ifstream file(resource_path, std::ios::binary);
        if (!file.is_open()) return false;
        unsigned char header[4];
        file.read(reinterpret_cast<char*>(header), 4);
        return (header[0] == 0x50 && header[1] == 0x4B && header[2] == 0x03 && header[3] == 0x04);
    }
}



// ----------------------------------------------------------------------------
// 内部卸载函数 (自动推断内存用量并同步状态)
// ----------------------------------------------------------------------------

void F_Resource::UnloadTextureById(const std::string& id) {
    auto tex_it = textures.find(id);
    if (tex_it != textures.end()) {
        if (m_enableMemoryMonitoring) {
            size_t memUsage = resourceStates.count(id) ? resourceStates[id].memory_usage_bytes : 0;
            if (memUsage > 0) m_currentMemoryUsage -= memUsage;
            resourceStates.erase(id);
        }
        UnloadTexture(tex_it->second);
        textures.erase(tex_it);
    }
}

void F_Resource::UnloadSoundById(const std::string& id) {
    auto snd_it = sounds.find(id);
    if (snd_it != sounds.end()) {
        if (m_enableMemoryMonitoring) {
            size_t memUsage = resourceStates.count(id) ? resourceStates[id].memory_usage_bytes : 0;
            if (memUsage > 0) m_currentMemoryUsage -= memUsage;
            resourceStates.erase(id);
        }
        UnloadSound(snd_it->second);
        sounds.erase(snd_it);
    }
}

void F_Resource::UnloadMusicById(const std::string& id) {
    auto mus_it = musics.find(id);
    if (mus_it != musics.end()) {
        if (m_enableMemoryMonitoring) {
            size_t memUsage = resourceStates.count(id) ? resourceStates[id].memory_usage_bytes : 0;
            if (memUsage > 0) m_currentMemoryUsage -= memUsage;
            resourceStates.erase(id);
            // 清理 Music 原始数据缓存
            datas.erase(id + "_internal_music_raw");
        }
        UnloadMusicStream(mus_it->second);
        musics.erase(mus_it);
    }
}

void F_Resource::UnloadFontById(const std::string& id) {
    auto fon_it = fonts.find(id);
    if (fon_it != fonts.end()) {
        if (m_enableMemoryMonitoring) {
            size_t memUsage = resourceStates.count(id) ? resourceStates[id].memory_usage_bytes : 0;
            if (memUsage > 0) m_currentMemoryUsage -= memUsage;
            resourceStates.erase(id);
        }
        UnloadFont(fon_it->second);
        fonts.erase(fon_it);
    }
}

void F_Resource::UnloadTextById(const std::string& id) {
    auto txt_it = texts.find(id);
    if (txt_it != texts.end()) {
        if (m_enableMemoryMonitoring) {
            size_t memUsage = resourceStates.count(id) ? resourceStates[id].memory_usage_bytes : 0;
            if (memUsage > 0) m_currentMemoryUsage -= memUsage;
            resourceStates.erase(id);
        }
        texts.erase(txt_it);
    }
}

void F_Resource::UnloadDataById(const std::string& id) {
    auto dat_it = datas.find(id);
    if (dat_it != datas.end()) {
        if (m_enableMemoryMonitoring) {
            size_t memUsage = resourceStates.count(id) ? resourceStates[id].memory_usage_bytes : 0;
            if (memUsage > 0) m_currentMemoryUsage -= memUsage;
            resourceStates.erase(id);
        }
        datas.erase(dat_it);
    }
}


void F_Resource::ExecuteUnload(const std::string& id, ResourceType type) {
    // 调用对应的卸载函数
    switch (type) {
    case ResTexture: UnloadTextureById(id); break;
    case ResSound: UnloadSoundById(id); break;
    case ResMusic: UnloadMusicById(id); break;
    case ResFont: UnloadFontById(id); break;
    case ResText: UnloadTextById(id); break;
    case ResData: UnloadDataById(id); break;
    default: return; // ResUnknown 或其他类型不处理
    }
}


// ----------------------------------------------------------------------------
// 公共接口：UnloadResource (处理引用计数)
// ----------------------------------------------------------------------------

void F_Resource::UnloadResource(const std::string& id, ResourceType type) {
    // 1. 如果是懒加载或批次标记，只移除标记，不卸载实际资源
    if (lazyLoadedResources.count(id)) {
        lazyLoadedResources.erase(id);
        return;
    }

    // 2. 检查内存监控是否开启
    if (m_enableMemoryMonitoring) {
        if (resourceStates.count(id)) {
            // 减少引用计数
            if (resourceStates[id].ref_count > 0) {
                resourceStates[id].ref_count--;
                // 如果引用计数仍大于 0，则不卸载
                if (resourceStates[id].ref_count > 0) return;
            }
        }
    }

    // 3. 引用计数为 0 或未开启内存监控，执行实际卸载
    ExecuteUnload(id, type);
}


// ----------------------------------------------------------------------------
// 公共接口：UnloadAllResources (强制卸载所有资源)
// ----------------------------------------------------------------------------

void F_Resource::UnloadAllResources() {

    // 强制卸载所有缓存中的资源
    for (auto& pair : textures) UnloadTexture(pair.second); textures.clear();
    for (auto& pair : sounds) UnloadSound(pair.second); sounds.clear();
    for (auto& pair : musics) {
        UnloadMusicStream(pair.second);
    }
    musics.clear();
    for (auto& pair : fonts) UnloadFont(pair.second); fonts.clear();

    texts.clear();
    datas.clear(); // 清理所有 datas，包括 Music 原始数据

    lazyLoadedResources.clear();
    resourceBatches.clear();

    // 清理内存监控状态
    if (m_enableMemoryMonitoring) {
        resourceStates.clear();
        m_currentMemoryUsage = 0;
    }
}