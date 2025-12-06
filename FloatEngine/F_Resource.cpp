#include "F_Resource.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <cstring> // for memcpy
#include "FloatApi.h"

// minizip-ng includes
#include "minizip-ng/mz.h"
#include "minizip-ng/mz_strm.h"
#include "minizip-ng/mz_strm_mem.h"
#include "minizip-ng/mz_zip.h"
#include "minizip-ng/mz_zip_rw.h"

// 哈希支持 pair<string, ResourceType>
namespace std {
    template <>
    struct hash<std::pair<std::string, ResourceType>> {
        std::size_t operator()(const std::pair<std::string, ResourceType>& p) const {
            return std::hash<std::string>()(p.first) ^ std::hash<int>()((int)p.second);
        }
    };
}

F_Resource::F_Resource() : use_memory_zip(false) {}
F_Resource::~F_Resource() { UnloadAllResources(); }

void F_Resource::SetResourcePath(const std::string& path) {
    resource_path = path;
    use_memory_zip = false;
    resource_data.clear();
}

void F_Resource::SetResourceData(const std::vector<unsigned char>& data) {
    resource_data = data;
    use_memory_zip = true;
    resource_path.clear();
}

// === 新增：设置密码 ===
void F_Resource::SetPassword(const std::string& password) {
    zip_password = password;
}

// === 实现：获取文件数量 ===
int F_Resource::GetFileCount() {
    void* zip_reader = mz_zip_reader_create();
    if (!zip_reader) return 0;

    int32_t err = MZ_OK;
    uint64_t count = 0;

    // 尝试打开流
    if (use_memory_zip) {
        void* mem_stream = mz_stream_mem_create();
        mz_stream_mem_set_buffer(mem_stream, (void*)resource_data.data(), (int32_t)resource_data.size());
        if (mz_zip_reader_open(zip_reader, mem_stream) == MZ_OK) {
            mz_zip_get_number_entry(zip_reader, &count);
        }
        mz_zip_reader_close(zip_reader); // Close implicitly closes stream if owned
        mz_stream_mem_delete(&mem_stream);
    }
    else {
        if (mz_zip_reader_open_file(zip_reader, resource_path.c_str()) == MZ_OK) {
            mz_zip_get_number_entry(zip_reader, &count);
        }
        mz_zip_reader_close(zip_reader);
    }

    mz_zip_reader_delete(&zip_reader);
    return (int)count;
}

// ----------- 核心：解压文件 (含加密支持) ----------------
std::vector<unsigned char> F_Resource::ExtractFileFromZip(const std::string& internalPath) {
    std::vector<unsigned char> data;
    void* zip_reader = mz_zip_reader_create();

    if (!zip_reader) {
        FLOG_ERRORF("Failed to create ZIP reader");
        return data;
    }

    // 1. 设置密码 (Minizip-ng 会根据此密码自动处理 AES 或 ZipCrypto)
    if (!zip_password.empty()) {
        mz_zip_reader_set_password(zip_reader, zip_password.c_str());
    }

    int32_t err = MZ_OK;
    void* mem_stream = nullptr;

    // 2. 打开 ZIP 来源
    if (use_memory_zip) {
        mem_stream = mz_stream_mem_create();
        if (!mem_stream) {
            mz_zip_reader_delete(&zip_reader);
            return data;
        }

        // 设置内存流 buffer
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

    // 3. 定位文件
    err = mz_zip_reader_locate_entry(zip_reader, internalPath.c_str(), 1); // 1 = ignore case

    if (err != MZ_OK) {
        FLOG_WARNF("File not found in ZIP: %s", internalPath.c_str());
    }
    else {
        // 4. 获取信息并解压
        mz_zip_file* file_info = nullptr;
        mz_zip_reader_entry_get_info(zip_reader, &file_info);

        if (file_info) {
            // 打开条目 (如果加密，此处会使用之前设置的密码进行验证)
            err = mz_zip_reader_entry_open(zip_reader);

            if (err == MZ_OK) {
                // 分配内存
                int64_t file_size = file_info->uncompressed_size;
                if (file_size > 0) {
                    data.resize((size_t)file_size);
                    int32_t bytes_read = mz_zip_reader_entry_read(zip_reader, data.data(), (int32_t)file_size);

                    if (bytes_read != file_size) {
                        FLOG_ERRORF("Error reading entry: read %d bytes, expected %lld", bytes_read, file_size);
                        data.clear();
                    }
                }
                mz_zip_reader_entry_close(zip_reader);
            }
            else {
                if (err == MZ_PASSWORD_ERROR) {
                    FLOG_ERRORF("Wrong password for file: %s", internalPath.c_str());
                }
                else {
                    FLOG_ERRORF("Failed to open zip entry: %s (Error: %d)", internalPath.c_str(), err);
                }
            }
        }
    }

    // 5. 清理
    mz_zip_reader_close(zip_reader);
    mz_zip_reader_delete(&zip_reader);
    if (mem_stream) mz_stream_mem_delete(&mem_stream);

    return data;
}

// ----------- 内部加载 (Raylib 对接) ----------------
bool F_Resource::LoadResourceInternal(const std::string& internalPath, const std::string& id,
    ResourceType type, int font_size, int* codepoints, int codepoints_size)
{
    auto fileData = ExtractFileFromZip(internalPath);
    if (fileData.empty()) return false;

    // 获取后缀名用于格式判断
    std::string ext = DetectFileFormat(fileData);

    switch (type) {
    case ResTexture: {
        Image img = LoadImageFromMemory(ext.c_str(), fileData.data(), (int)fileData.size());
        if (img.data != nullptr) {
            Texture2D tex = LoadTextureFromImage(img);
            UnloadImage(img);
            textures[id] = tex;
            paths[id] = internalPath;
            return true;
        }
        break;
    }
    case ResSound: {
        Wave wave = LoadWaveFromMemory(ext.c_str(), fileData.data(), (int)fileData.size());
        if (wave.data != nullptr) {
            Sound sound = LoadSoundFromWave(wave);
            UnloadWave(wave);
            if (sound.stream.buffer != nullptr) {
                sounds[id] = sound;
                paths[id] = internalPath;
                return true;
            }
        }
        break;
    }
    case ResMusic: {
        // [关键修复] Music 是流媒体，Raylib 不会拷贝数据，只是持有指针。
        // fileData 在函数结束后会析构，导致野指针崩溃。
        // 我们必须将原始数据持久化到 datas 容器中。
        std::string rawKey = id + "_internal_music_raw";
        datas[rawKey] = fileData;

        Music music = LoadMusicStreamFromMemory(ext.c_str(), datas[rawKey].data(), (int)datas[rawKey].size());

        if (music.stream.buffer != nullptr) {
            musics[id] = music;
            paths[id] = internalPath;
            return true;
        }
        else {
            // 加载失败，清理缓存
            datas.erase(rawKey);
        }
        break;
    }
    case ResFont: {
        Font font = LoadFontFromMemory(ext.c_str(), fileData.data(), (int)fileData.size(), font_size, codepoints, codepoints_size);
        if (font.texture.id != 0) {
            fonts[id] = font;
            paths[id] = internalPath;
            return true;
        }
        break;
    }
    case ResText: {
        // 确保字符串以 \0 结尾
        if (fileData.empty() || fileData.back() != '\0') fileData.push_back('\0');
        texts[id] = std::string(reinterpret_cast<char*>(fileData.data()));
        paths[id] = internalPath;
        return true;
    }
    case ResData: {
        datas[id] = fileData;
        paths[id] = internalPath;
        return true;
    }
    }

    FLOG_ERRORF("Failed to load resource: %s type: %d", internalPath.c_str(), type);
    return false;
}

// ----------- 懒加载与普通加载 ----------------
bool F_Resource::LoadResource(const std::string& internalPath, const std::string& id, ResourceType type, bool lazyLoad) {
    if (lazyLoad) {
        lazyLoadedResources.insert({ id, type });
        paths[id] = internalPath;
        return true;
    }
    else {
        return LoadResourceInternal(internalPath, id, type);
    }
}

bool F_Resource::LoadFontResource(const std::string& internalPath, const std::string& id, int font_size, int* codepoints, int codepoints_size, bool lazyLoad) {
    if (lazyLoad) {
        lazyLoadedResources.insert({ id, ResFont });
        paths[id] = internalPath; // 修正 insert 语法
        return true;
    }
    else {
        return LoadResourceInternal(internalPath, id, ResFont, font_size, codepoints, codepoints_size);
    }
}

// ----------- 批量管理 ----------------
bool F_Resource::CreateLoadBatch(const std::string& batchId, const std::vector<std::pair<std::string, ResourceType>>& resources) {
    resourceBatches[batchId] = std::set<std::pair<std::string, ResourceType>>(resources.begin(), resources.end());
    return true;
}

bool F_Resource::LoadResourceToBatch(const std::string& internalPath, const std::string& id, ResourceType type, const std::string& batchId) {
    paths[id] = internalPath;
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) {
        FLOG_ERRORF("Batch '%s' not found!", batchId.c_str());
        return false;
    }
    auto resourcePair = std::make_pair(id, type);
    if (batchIt->second.count(resourcePair)) { return false; }
    batchIt->second.insert(resourcePair);
    return true;
}

bool F_Resource::LoadFontResourceToBatch(const std::string& internalPath, const std::string& id, int font_size, const std::string& batchId, int* codepoints, int codepoints_size) {
    paths[id] = internalPath;
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) return false;

    // 这里暂时无法存储 font_size 参数用于延迟加载，建议后续扩展结构体存储 Font 参数
    // 目前仅做简单插入
    auto resourcePair = std::make_pair(id, ResFont);
    if (batchIt->second.count(resourcePair)) return false;

    batchIt->second.insert(resourcePair);
    return true;
}

void F_Resource::LoadBatchResources(const std::string& batchId) {
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) return;

    for (const auto& resourcePair : batchIt->second) {
        const std::string& resourceId = resourcePair.first;
        ResourceType resourceType = resourcePair.second;

        if (HasResource(resourceId, resourceType)) continue;

        std::string resourcePath = GetResourcePath(resourceId);
        if (resourcePath.empty()) continue;

        // 注意：这里批量加载字体时会丢失 font_size 信息，使用的是默认值0
        // 如果需要完美支持批量加载特定大小字体，需要修改数据结构存储参数
        LoadResourceInternal(resourcePath, resourceId, resourceType);
    }
}

void F_Resource::UnloadBatchResources(const std::string& batchId) {
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) return;

    for (const auto& resourcePair : batchIt->second) {
        UnloadResource(resourcePair.first, resourcePair.second);
    }
}

// ----------- 资源获取 ----------------
Texture2D F_Resource::GetTexture(const std::string& id) { return textures.count(id) ? textures[id] : Texture2D{}; }
Sound F_Resource::GetSound(const std::string& id) { return sounds.count(id) ? sounds[id] : Sound{}; }
Music F_Resource::GetMusic(const std::string& id) { return musics.count(id) ? musics[id] : Music{}; }
Font F_Resource::GetFont(const std::string& id) { return fonts.count(id) ? fonts[id] : Font{}; }
std::string F_Resource::GetText(const std::string& id) { return texts.count(id) ? texts[id] : ""; }
std::vector<unsigned char> F_Resource::GetData(const std::string& id) { return datas.count(id) ? datas[id] : std::vector<unsigned char>(); }

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

std::string F_Resource::GetResourcePath(const std::string& id) {
    return paths.count(id) ? paths[id] : "";
}

// ----------- 资源卸载 ----------------
void F_Resource::UnloadResource(const std::string& id, ResourceType type) {
    // 如果在懒加载队列中，直接移除
    if (lazyLoadedResources.count({ id, type })) {
        lazyLoadedResources.erase({ id, type });
        return;
    }

    switch (type) {
    case ResTexture: UnloadTextureById(id); break;
    case ResSound: UnloadSoundById(id); break;
    case ResMusic: UnloadMusicById(id); break;
    case ResFont: UnloadFontById(id); break;
    case ResText: UnloadTextById(id); break;
    case ResData: UnloadDataById(id); break;
    }
}

void F_Resource::UnloadAllResources() {
    for (auto& pair : textures) UnloadTexture(pair.second); textures.clear();
    for (auto& pair : sounds) UnloadSound(pair.second); sounds.clear();
    for (auto& pair : musics) {
        UnloadMusicStream(pair.second);
        // 同时清理关联的 music_raw 数据
        datas.erase(pair.first + "_internal_music_raw");
    }
    musics.clear();
    for (auto& pair : fonts) UnloadFont(pair.second); fonts.clear();
    texts.clear();
    datas.clear();
    paths.clear();
    lazyLoadedResources.clear();
}

// ----------- 具体的卸载实现 ----------------
void F_Resource::UnloadTextureById(const std::string& id) {
    auto it = textures.find(id);
    if (it != textures.end()) {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

void F_Resource::UnloadSoundById(const std::string& id) {
    auto it = sounds.find(id);
    if (it != sounds.end()) {
        UnloadSound(it->second);
        sounds.erase(it);
    }
}

void F_Resource::UnloadMusicById(const std::string& id) {
    auto it = musics.find(id);
    if (it != musics.end()) {
        UnloadMusicStream(it->second);
        musics.erase(it);
        // 清理绑定的原始数据
        datas.erase(id + "_internal_music_raw");
    }
}

void F_Resource::UnloadFontById(const std::string& id) {
    auto it = fonts.find(id);
    if (it != fonts.end()) {
        UnloadFont(it->second);
        fonts.erase(it);
    }
}

void F_Resource::UnloadTextById(const std::string& id) {
    texts.erase(id);
}

void F_Resource::UnloadDataById(const std::string& id) {
    datas.erase(id);
}

// ----------- 辅助：格式检测 ----------------
std::string F_Resource::DetectFileFormat(const std::vector<unsigned char>& data) {
    if (data.size() < 4) return ".dat";

    const unsigned char* bytes = data.data();

    // 简单魔数检测
    if (bytes[0] == 0x89 && bytes[1] == 0x50 && bytes[2] == 0x4E && bytes[3] == 0x47) return ".png";
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
