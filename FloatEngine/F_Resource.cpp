#include "F_Resource.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include "FloatApi.h"

#include "minizip-ng/mz.h"
#include "minizip-ng/mz_strm.h"
#include "minizip-ng/mz_crypt.h"
#include "minizip-ng/mz_strm_split.h"
#include "minizip-ng/mz_strm_mem.h"
#include "minizip-ng/mz_strm_os.h"
#include "minizip-ng/mz_zip.h"
#include "minizip-ng/mz_zip_rw.h"
#include "minizip-ng/ioapi.h"
#include "minizip-ng/mz_strm_bzip.h"

struct ZipReader {
    std::string path;
    std::vector<unsigned char> data;
    bool is_open = false;
};

// 哈希支持 pair<string, ResourceType> 用于 unordered_set/map
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

void F_Resource::SetResourcePath(const std::string& path) { resource_path = path; use_memory_zip = false; }
void F_Resource::SetResourceData(const std::vector<unsigned char>& data) { resource_data = data; use_memory_zip = true; }

int F_Resource::GetFileCount() { return 0; } // 简化

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
        paths.insert({ internalPath,id });
    }
    else {
        return LoadResourceInternal(internalPath, id, ResFont, font_size, codepoints, codepoints_size);
    }
}

// ----------- 批量加载 ----------------
bool F_Resource::CreateLoadBatch(const std::string& batchId, const std::vector<std::pair<std::string, ResourceType>>& resources) {
    resourceBatches[batchId] = std::set<std::pair<std::string, ResourceType>>(resources.begin(), resources.end());
    return true;
}

bool F_Resource::LoadFontResourceToBatch(const std::string& internalPath, const std::string& id, int font_size, const std::string& batchId, int* codepoints, int codepoints_size) {
    paths[id] = internalPath;
    // 检查批次是否存在
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) {
        FLOG_ERRORF("Batch '%s' not found!", batchId.c_str());
        return false;
    }

    // 尝试添加资源到批次中
    auto resourcePair = std::make_pair(id, ResFont);
    if (batchIt->second.count(resourcePair)) {
        FLOG_ERRORF("Font resource '%s' already exists in batch '%s'", id.c_str(), batchId.c_str());
        return false;
    }

    batchIt->second.insert(resourcePair);
    FLOG_INFOF("Successfully loaded font resource '%s' into batch '%s'", id.c_str(), batchId.c_str());
    return true;

}

bool F_Resource::LoadResourceToBatch(const std::string& internalPath, const std::string& id, ResourceType type, const std::string& batchId) {
    paths[id] = internalPath;
    // 检查批次是否存在
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) {
        FLOG_ERRORF("Batch '%s' not found!", batchId.c_str());
        return false;
    }

    // 尝试添加资源到批次中
    auto resourcePair = std::make_pair(id, type);
    if (batchIt->second.count(resourcePair)) {
        FLOG_ERRORF("Resource '%s' (Type: %d) already exists in batch '%s'", id.c_str(), type, batchId.c_str());
        return false;
    }

    batchIt->second.insert(resourcePair);
    FLOG_INFOF("Successfully loaded resource '%s' (Type: %d) into batch '%s'", id.c_str(), type, batchId.c_str());
    return true;

}

void F_Resource::LoadBatchResources(const std::string& batchId) {
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) {
        FLOG_ERRORF("Batch '%s' not found!", batchId.c_str());
        return;
    }

    // 遍历批次中的所有资源并加载
    for (const auto& resourcePair : batchIt->second) {
        const std::string& resourceId = resourcePair.first;
        ResourceType resourceType = resourcePair.second;

        // 检查资源是否已加载
        if (HasResource(resourceId, resourceType)) {
            FLOG_INFOF("Resource '%s' already loaded, skipping...", resourceId.c_str());
            continue;
        }

        // 根据资源类型加载（从对应的路径）
        std::string resourcePath = GetResourcePath(resourceId);
        if (resourcePath.length() <= 1) {
            FLOG_WARNF("Warning: Resource path not found for '%s'  paths[%s]=%s", resourceId.c_str(), resourceId.c_str(), paths[resourceId].c_str());
            continue;
        }

        bool success = false;
        success = LoadResourceInternal(resourcePath, resourceId, resourceType);

        if (success) {
            FLOG_INFOF("Loaded resource: %s (Type: %d)", resourceId.c_str(), resourceType);
        }
        else {
            FLOG_ERRORF("Failed to load resource: %s,  ResType=%d", resourceId.c_str(), resourceType);
        }
    }
}

void F_Resource::UnloadBatchResources(const std::string& batchId) {
    auto batchIt = resourceBatches.find(batchId);
    if (batchIt == resourceBatches.end()) {
        FLOG_ERRORF("Batch '%s' not found!", batchId.c_str());
        return;
    }

    // 遍历批次中的所有资源并卸载
    for (const auto& resourcePair : batchIt->second) {
        const std::string& resourceId = resourcePair.first;
        ResourceType resourceType = resourcePair.second;

        // 检查资源是否存在
        if (!HasResource(resourceId, resourceType)) {
            FLOG_INFOF("Resource '%s' not loaded, skipping...", resourceId.c_str());
            continue;
        }

        // 卸载资源
        UnloadResource(resourceId, resourceType);
        FLOG_INFOF("Unloaded resource: %s (Type: %d)", resourceId.c_str(), resourceType);
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

void F_Resource::UnloadResource(const std::string& id, ResourceType type) {
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

// ----------- 卸载全部 ----------------
void F_Resource::UnloadAllResources() {
    for (auto& pair : textures) UnloadTexture(pair.second); textures.clear();
    for (auto& pair : sounds) UnloadSound(pair.second); sounds.clear();
    for (auto& pair : musics) UnloadMusicStream(pair.second); musics.clear();
    for (auto& pair : fonts) UnloadFont(pair.second); fonts.clear();
    texts.clear(); datas.clear(); paths.clear();
}

// ----------- 内部加载 ----------------
bool F_Resource::LoadResourceInternal(const std::string& internalPath, const std::string& id,
    ResourceType type, int font_size, int* codepoints, int codepoints_size)
{
    auto fileData = ExtractFileFromZip(internalPath);
    if (fileData.empty()) return false;

    switch (type) {
    case ResTexture: {
        Image img = LoadImageFromMemory(DetectFileFormat(fileData).c_str(), fileData.data(), fileData.size());
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
        Wave wave = LoadWaveFromMemory(DetectFileFormat(fileData).c_str(), fileData.data(), fileData.size());
        Sound sound = LoadSoundFromWave(wave);
        UnloadWave(wave);
        if (sound.stream.buffer != nullptr) { sounds[id] = sound; paths[id] = internalPath; return true; }
        break;
    }
    case ResMusic: {
        Music music = LoadMusicStreamFromMemory(DetectFileFormat(fileData).c_str(), fileData.data(), fileData.size());
        musics[id] = music; paths[id] = internalPath; break;
    }
    case ResFont: {
        Font font = LoadFontFromMemory(DetectFileFormat(fileData).c_str(), fileData.data(), fileData.size(), font_size, codepoints, codepoints_size);
        if (font.texture.id != 0) { fonts[id] = font; paths[id] = internalPath; return true; }
        break;
    }
    case ResText: {
        texts[id] = std::string(fileData.begin(), fileData.end());
        paths[id] = internalPath;
        return true;
    }
    case ResData: {
        datas[id] = fileData;
        paths[id] = internalPath;
        return true;
    }
    }
    return false;
}
// 内存读取回调结构
struct MemoryStream {
    const unsigned char* data;
    int64_t size;
    int64_t offset;
};
// 内存流回调函数实现
static int32_t memory_stream_open(void* stream, const char* path, int32_t mode) {
    // 对于内存流，open 操作已经在初始化时完成
    return MZ_OK;
}

static int32_t memory_stream_is_open(void* stream) {
    MemoryStream* mem_stream = (MemoryStream*)stream;
    return (mem_stream != nullptr && mem_stream->data != nullptr) ? 1 : 0;
}

static int32_t memory_stream_read(void* stream, void* buf, int32_t size) {
    MemoryStream* mem_stream = (MemoryStream*)stream;
    if (!mem_stream || !mem_stream->data) return 0;

    int64_t max_available = mem_stream->size - mem_stream->offset;
    if (max_available <= 0) return 0;

    int32_t bytes_to_read = (int32_t)((size < max_available) ? size : max_available);

    if (bytes_to_read > 0) {
        memcpy(buf, mem_stream->data + mem_stream->offset, bytes_to_read);
        mem_stream->offset += bytes_to_read;
        return bytes_to_read;
    }
    return 0;
}

static int32_t memory_stream_write(void* stream, const void* buf, int32_t size) {
    // 只读操作，不需要实现
    return 0;
}

static int64_t memory_stream_tell(void* stream) {
    MemoryStream* mem_stream = (MemoryStream*)stream;
    return mem_stream ? mem_stream->offset : 0;
}

static int32_t memory_stream_seek(void* stream, int64_t offset, int32_t origin) {
    MemoryStream* mem_stream = (MemoryStream*)stream;
    if (!mem_stream) return MZ_SEEK_ERROR;

    int64_t new_offset = mem_stream->offset;
    switch (origin) {
    case MZ_SEEK_SET:
        new_offset = offset;
        break;
    case MZ_SEEK_CUR:
        new_offset = mem_stream->offset + offset;
        break;
    case MZ_SEEK_END:
        new_offset = mem_stream->size + offset;
        break;
    default:
        return MZ_SEEK_ERROR;
    }

    if (new_offset < 0 || new_offset > mem_stream->size) {
        return MZ_SEEK_ERROR;
    }

    mem_stream->offset = new_offset;
    return MZ_OK;
}

static int32_t memory_stream_close(void* stream) {
    // 不需要特殊清理
    return MZ_OK;
}

static int32_t memory_stream_error(void* stream) {
    return MZ_OK;
}

static void* memory_stream_create(void) {
    MemoryStream* mem_stream = new MemoryStream();
    mem_stream->data = nullptr;
    mem_stream->size = 0;
    mem_stream->offset = 0;
    return mem_stream;
}

static void memory_stream_destroy(void** stream) {
    if (stream && *stream) {
        MemoryStream* mem_stream = (MemoryStream*)*stream;
        delete mem_stream;
        *stream = nullptr;
    }
}

// 使用正确的回调类型定义，添加强制转换
static mz_stream_vtbl memory_stream_vtbl = {
    (mz_stream_open_cb)memory_stream_open,
    (mz_stream_is_open_cb)memory_stream_is_open,
    (mz_stream_read_cb)memory_stream_read,
    (mz_stream_write_cb)memory_stream_write,
    (mz_stream_tell_cb)memory_stream_tell,
    (mz_stream_seek_cb)memory_stream_seek,
    (mz_stream_close_cb)memory_stream_close,
    (mz_stream_error_cb)memory_stream_error,
    (mz_stream_create_cb)memory_stream_create,
    (mz_stream_destroy_cb)memory_stream_destroy
};
std::vector<unsigned char> F_Resource::ExtractFileFromZip(const std::string& internalPath) {
    std::vector<unsigned char> data;

    FLOG_DEBUGF("Extracting file from ZIP using proper memory stream: %s", internalPath.c_str());

    // 创建ZIP阅读器
    void* zip_reader = mz_zip_reader_create();
    if (!zip_reader) {
        FLOG_ERRORF("Failed to create ZIP reader");
        return data;
    }

    int32_t err = MZ_OK;

    if (use_memory_zip) {
        // 使用minizip-ng提供的内存流 - 这是正确的方法
        void* mem_stream = mz_stream_mem_create();
        if (!mem_stream) {
            FLOG_ERRORF("Failed to create memory stream");
            mz_zip_reader_delete(&zip_reader);
            return data;
        }

        // 设置内存缓冲区
        mz_stream_mem_set_buffer(mem_stream, (void*)resource_data.data(), resource_data.size());
        if (resource_data.empty()) {
            FLOG_ERRORF("Failed to set memory stream buffer: %d", err);
            mz_stream_mem_delete(&mem_stream);
            mz_zip_reader_delete(&zip_reader);
            return data;
        }

        // 打开内存流进行读取
        err = mz_stream_mem_open(mem_stream, NULL, MZ_OPEN_MODE_READ);
        if (err != MZ_OK) {
            FLOG_ERRORF("Failed to open memory stream: %d", err);
            mz_stream_mem_delete(&mem_stream);
            mz_zip_reader_delete(&zip_reader);
            return data;
        }

        // 使用内存流打开ZIP阅读器
        err = mz_zip_reader_open(zip_reader, mem_stream);
        if (err != MZ_OK) {
            FLOG_ERRORF("Failed to open ZIP reader with memory stream: %d", err);
            mz_stream_mem_close(mem_stream);
            mz_stream_mem_delete(&mem_stream);
            mz_zip_reader_delete(&zip_reader);
            return data;
        }

    }
    else {
        // 文件ZIP的打开方式保持不变
        err = mz_zip_reader_open_file(zip_reader, resource_path.c_str());
        if (err != MZ_OK) {
            FLOG_ERRORF("Failed to open ZIP file: %s, error: %d", resource_path.c_str(), err);
            mz_zip_reader_delete(&zip_reader);
            return data;
        }
    }

    // 获取文件数量用于调试
    uint64_t file_count = 0;
    err = mz_zip_get_number_entry(zip_reader, &file_count);
    if (err == MZ_OK) {
        FLOG_DEBUGF("ZIP contains %llu entries", file_count);
    }

    // 定位文件
    err = mz_zip_reader_locate_entry(zip_reader, internalPath.c_str(), 0); // 0 = case insensitive
    if (err != MZ_OK) {
        FLOG_ERRORF("Failed to locate file in ZIP: %s, error: %d", internalPath.c_str(), err);
        mz_zip_reader_close(zip_reader);
        mz_zip_reader_delete(&zip_reader);
        return data;
    }

    FLOG_DEBUGF("File located successfully: %s", internalPath.c_str());

    // 获取文件信息
    mz_zip_file* file_info = nullptr;
    mz_zip_reader_entry_get_info(zip_reader, &file_info);
    if (file_info) {
        FLOG_DEBUGF("File info - Compression: %d, Flags: %d, Size: %lld",
            file_info->compression_method, file_info->flag, file_info->uncompressed_size);

        // 检查是否加密
        if (file_info->flag & MZ_ZIP_FLAG_ENCRYPTED) {
            FLOG_ERRORF("File is encrypted, cannot extract");
            mz_zip_reader_close(zip_reader);
            mz_zip_reader_delete(&zip_reader);
            return data;
        }
    }

    // 打开文件条目
    err = mz_zip_reader_entry_open(zip_reader);
    if (err != MZ_OK) {
        FLOG_ERRORF("Failed to open ZIP entry: %s, error: %d", internalPath.c_str(), err);

        // 检查是否是压缩方法不支持
        if (err == MZ_SUPPORT_ERROR) {
            FLOG_ERRORF("Unsupported compression method: %d", file_info ? file_info->compression_method : -1);
        }

        mz_zip_reader_close(zip_reader);
        mz_zip_reader_delete(&zip_reader);
        return data;
    }

    FLOG_DEBUGF("ZIP entry opened successfully");

    // 读取文件数据
    if (file_info && file_info->uncompressed_size > 0) {
        int64_t file_size = file_info->uncompressed_size;
        data.resize(file_size);

        int32_t bytes_read = mz_zip_reader_entry_read(zip_reader, data.data(), (int32_t)file_size);

        if (bytes_read == file_size) {
            FLOG_INFOF("Successfully extracted: %s (%lld bytes)", internalPath.c_str(), file_size);
        }
        else {
            FLOG_ERRORF("Failed to read file: %s, expected %lld, got %d",
                internalPath.c_str(), file_size, bytes_read);
            data.clear();
        }
    }
    else {
        FLOG_ERRORF("Invalid file size for: %s", internalPath.c_str());
    }

    // 关闭条目和阅读器
    mz_zip_reader_entry_close(zip_reader);
    mz_zip_reader_close(zip_reader);
    mz_zip_reader_delete(&zip_reader);

    return data;
}
bool F_Resource::IsValidZipFile() const {
    if (use_memory_zip) {
        if (resource_data.empty()) {
            FLOG_ERRORF("Memory ZIP data is empty");
            return false;
        }

        // 更严格的ZIP文件检查
        if (resource_data.size() < 4) {
            FLOG_ERRORF("Memory ZIP data too small: %zu bytes", resource_data.size());
            return false;
        }

        // 检查ZIP文件签名
        bool has_valid_signature = (resource_data[0] == 0x50 && resource_data[1] == 0x4B &&
            resource_data[2] == 0x03 && resource_data[3] == 0x04);

        if (!has_valid_signature) {
            FLOG_ERRORF("Invalid ZIP signature in memory data");
            // 输出前几个字节用于调试
            FLOG_DEBUGF("First 4 bytes: %02X %02X %02X %02X",
                resource_data[0], resource_data[1], resource_data[2], resource_data[3]);
        }

        return has_valid_signature;
    }
    else {
        // 检查文件是否存在且可读
        std::ifstream file(resource_path, std::ios::binary);
        if (!file.is_open()) {
            FLOG_ERRORF("Cannot open ZIP file: %s", resource_path.c_str());
            return false;
        }

        // 检查文件大小
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        if (file_size < 4) {
            FLOG_ERRORF("ZIP file too small: %zu bytes", file_size);
            file.close();
            return false;
        }

        // 读取并检查文件头
        unsigned char header[4];
        file.read(reinterpret_cast<char*>(header), 4);
        file.close();

        bool isValid = (header[0] == 0x50 && header[1] == 0x4B &&
            header[2] == 0x03 && header[3] == 0x04);

        if (!isValid) {
            FLOG_ERRORF("Invalid ZIP file signature in: %s", resource_path.c_str());
            FLOG_DEBUGF("First 4 bytes: %02X %02X %02X %02X", header[0], header[1], header[2], header[3]);
        }

        return isValid;
    }
}
std::string F_Resource::DetectFileFormat(const std::vector<unsigned char>& data) {
    if (data.size() < 8) {
        FLOG_WARNF("File data too small to detect format, size: %zu", data.size());
        return "";
    }

    // 检查常见文件格式的魔数
    const unsigned char* bytes = data.data();

    // PNG
    if (bytes[0] == 0x89 && bytes[1] == 0x50 && bytes[2] == 0x4E && bytes[3] == 0x47) {
        FLOG_DEBUGF("Detected PNG format");
        return ".png";
    }

    // JPEG
    if (bytes[0] == 0xFF && bytes[1] == 0xD8 && bytes[2] == 0xFF) {
        FLOG_DEBUGF("Detected JPEG format");
        return ".jpg";
    }

    // GIF
    if (bytes[0] == 0x47 && bytes[1] == 0x49 && bytes[2] == 0x46) {
        FLOG_DEBUGF("Detected GIF format");
        return ".gif";
    }

    // BMP
    if (bytes[0] == 0x42 && bytes[1] == 0x4D) {
        FLOG_DEBUGF("Detected BMP format");
        return ".bmp";
    }

    // WAV
    if (bytes[0] == 0x52 && bytes[1] == 0x49 && bytes[2] == 0x46 && bytes[3] == 0x46 &&
        bytes[8] == 0x57 && bytes[9] == 0x41 && bytes[10] == 0x56 && bytes[11] == 0x45) {
        FLOG_DEBUGF("Detected WAV format");
        return ".wav";
    }

    // OGG
    if (bytes[0] == 0x4F && bytes[1] == 0x67 && bytes[2] == 0x67 && bytes[3] == 0x53) {
        FLOG_DEBUGF("Detected OGG format");
        return ".ogg";
    }

    // MP3 (ID3v2 header)
    if (bytes[0] == 0x49 && bytes[1] == 0x44 && bytes[2] == 0x33) {
        FLOG_DEBUGF("Detected MP3 format");
        return ".mp3";
    }

    // MP3 (frame sync)
    if ((bytes[0] == 0xFF) && ((bytes[1] & 0xE0) == 0xE0)) {
        FLOG_DEBUGF("Detected MP3 format (frame sync)");
        return ".mp3";
    }

    // TTF/OTF
    if (bytes[0] == 0x00 && bytes[1] == 0x01 && bytes[2] == 0x00 && bytes[3] == 0x00) {
        FLOG_DEBUGF("Detected TTF/OTF font format");
        return ".ttf";
    }

    // WOFF
    if (bytes[0] == 0x77 && bytes[1] == 0x4F && bytes[2] == 0x46 && bytes[3] == 0x46) {
        FLOG_DEBUGF("Detected WOFF font format");
        return ".woff";
    }

    FLOG_WARNF("Unknown file format, using default extension");
    return ".dat"; // 默认扩展名
}

void F_Resource::UnloadTextureById(const std::string& id) {
    auto it = textures.find(id);
    if (it != textures.end()) {
        if (it->second.id != 0) {
            UnloadTexture(it->second);
            FLOG_DEBUGF("Unloaded texture: %s", id.c_str());
        }
        textures.erase(it);
    }
    else {
        FLOG_WARNF("Texture not found for unloading: %s", id.c_str());
    }
}

void F_Resource::UnloadSoundById(const std::string& id) {
    auto it = sounds.find(id);
    if (it != sounds.end()) {
        if (it->second.stream.buffer != nullptr) {
            UnloadSound(it->second);
            FLOG_DEBUGF("Unloaded sound: %s", id.c_str());
        }
        sounds.erase(it);
    }
    else {
        FLOG_WARNF("Sound not found for unloading: %s", id.c_str());
    }
}

void F_Resource::UnloadMusicById(const std::string& id) {
    auto it = musics.find(id);
    if (it != musics.end()) {
        if (it->second.stream.buffer != nullptr) {
            UnloadMusicStream(it->second);
            FLOG_DEBUGF("Unloaded music: %s", id.c_str());
        }
        musics.erase(it);
    }
    else {
        FLOG_WARNF("Music not found for unloading: %s", id.c_str());
    }
}

void F_Resource::UnloadFontById(const std::string& id) {
    auto it = fonts.find(id);
    if (it != fonts.end()) {
        if (it->second.texture.id != 0) {
            UnloadFont(it->second);
            FLOG_DEBUGF("Unloaded font: %s", id.c_str());
        }
        fonts.erase(it);
    }
    else {
        FLOG_WARNF("Font not found for unloading: %s", id.c_str());
    }
}

void F_Resource::UnloadTextById(const std::string& id) {
    auto it = texts.find(id);
    if (it != texts.end()) {
        texts.erase(it);
        FLOG_DEBUGF("Unloaded text: %s", id.c_str());
    }
    else {
        FLOG_WARNF("Text not found for unloading: %s", id.c_str());
    }
}

void F_Resource::UnloadDataById(const std::string& id) {
    auto it = datas.find(id);
    if (it != datas.end()) {
        datas.erase(it);
        FLOG_DEBUGF("Unloaded data: %s", id.c_str());
    }
    else {
        FLOG_WARNF("Data not found for unloading: %s", id.c_str());
    }
}
// ----------- minizip-ng 简化 ----------------
void* F_Resource::CreateZipReader() { return new ZipReader(); }
void F_Resource::DeleteZipReader(void* reader) { delete static_cast<ZipReader*>(reader); }
bool F_Resource::OpenZipReader(void* reader) { auto r = static_cast<ZipReader*>(reader); r->is_open = true; return true; }
void F_Resource::CloseZipReader(void* reader) { auto r = static_cast<ZipReader*>(reader); r->is_open = false; }

std::string F_Resource::GetResourcePath(const std::string& id) {
    return paths[id].length() > 1 ? paths[id] : "";
}