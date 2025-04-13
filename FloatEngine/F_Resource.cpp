#include "F_Resource.h"
#include "minizip/unzip.h"
#include "minizip/zip.h"
#include <zlib.h>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <filesystem>
extern void DEBUG_LOG(int lv, const char* text, bool english = 1, bool auto_enter = 1);


zlib_filefunc64_def _mem_zip_io = { 0 }; // 内存 ZIP 的 IO 回调

static unsigned long ZCALLBACK mem_read(void* opaque, void* buf, unsigned long size) {
    MemoryBuffer* mem = (MemoryBuffer*)opaque;
    size_t bytes_to_read = (mem->position + size <= mem->size) ? size : mem->size - mem->position;
    if (bytes_to_read > 0) {
        memcpy(buf, mem->buffer + mem->position, bytes_to_read);
        mem->position += bytes_to_read;
    }
    return (unsigned long)bytes_to_read;
}

// 自定义定位函数
static long ZCALLBACK mem_seek(void* opaque, unsigned long offset, int origin) {
    MemoryBuffer* mem = (MemoryBuffer*)opaque;
    switch (origin) {
    case ZLIB_FILEFUNC_SEEK_SET:
        mem->position = offset;
        break;
    case ZLIB_FILEFUNC_SEEK_CUR:
        mem->position += offset;
        break;
    case ZLIB_FILEFUNC_SEEK_END:
        mem->position = mem->size + offset;
        break;
    default:
        return -1;
    }
    return 0;
}

// 构造函数
F_Resource::F_Resource() : _password(nullptr) {
    has_password = 0;
}

// 析构函数
F_Resource::~F_Resource() {
    UnloadAllResources();
}

// 设置zip路径
void F_Resource::SetZipPath(const std::string& path) {
    _use_memory_zip = false;  // 切换到文件模式
    _zip_path = path;
    // 检查文件是否存在
    std::ifstream file(_zip_path);
    if (file.good()) {
          DEBUG_LOG(LOG_INFO,
              TextFormat("F_Resource.SetZipPath:压缩文件%s成功打开",path.c_str()), 0);
    }
    else {
        DEBUG_LOG(LOG_ERROR,"F_Resource.SetZipPath:文件不存在", 0);
    }
}

void F_Resource::SetZipData(std::vector<unsigned char> data) {
    if (data.empty()) {
        DEBUG_LOG(LOG_WARNING, "传入的 ZIP 数据为空", 0);
        return;
    }
    _zip_data = std::move(data);
    _use_memory_zip = true;  // 标记使用内存 ZIP

    // 初始化内存 ZIP 的 IO 回调
    _mem_zip_io.zopen64_file = nullptr;
    _mem_zip_io.zread_file = (read_file_func)mem_read;
    _mem_zip_io.zwrite_file = nullptr;
    _mem_zip_io.ztell64_file = nullptr;
    _mem_zip_io.zseek64_file = (seek64_file_func)mem_seek;
    _mem_zip_io.zclose_file = nullptr;
    _mem_zip_io.zerror_file = nullptr;
    _mem_zip_io.opaque = &_zip_data;  // 传递 ZIP 数据指针
}

// 设置压缩包密码
void F_Resource::SetPassword(const char* password) {
    _password = password;
    has_password = true;
}

void F_Resource::NoPassword()
{
    has_password = false;
}

// 加载资源
bool F_Resource::LoadResource(const std::string& path, const std::string& id, ResourceType type) {
    switch (type) {
    case ResTexture:
        return LoadTextureFromZlib(path, id);
    case ResSound:
        return LoadSoundFromZlib(path, id);
    case ResMusic:
        return LoadMusicFromZlib(path, id);
    case ResText:
        return LoadTextFromZlib(path, id);
    case ResData:
        //读取数据
        return LoadDataFromZlib(path, id);
    default:
        return false;
    }
}
\

// 加载字体资源
bool F_Resource::LoadFontResource(const std::string& path, const std::string& id, int font_size, int* codepoints, int codepoints_size) {
    return LoadFontFromZlib(path, id, font_size, codepoints, codepoints_size);
}
\

// 加入资源
void F_Resource::AddResource(const std::string& id, ResourceType type, void* resource) {
    switch (type) {
    case ResTexture:
        textures[id] = *static_cast<Texture2D*>(resource);
        break;
    case ResSound:
        sounds[id] = *static_cast<Sound*>(resource);
        break;
    case ResMusic:
        musics[id] = *static_cast<Music*>(resource);
        break;
    case ResFont:
        fonts[id] = *static_cast<Font*>(resource);
        break;
    case ResText:
        texts[id] = *static_cast<std::string*>(resource);
        break;
    case ResData:
		datas[id] = *static_cast<std::vector<unsigned char>*>(resource);
		break;
    default:
        break;
    }
}

// 获取资源
Texture2D F_Resource::GetTexture(const std::string& id) {
    return textures[id];
}

Sound F_Resource::GetSound(const std::string& id) {
    return sounds[id];
}

Music F_Resource::GetMusic(const std::string& id) {
    return musics[id];
}

Font F_Resource::GetFont(const std::string& id) {
    return fonts[id];
}

std::string F_Resource::GetText(const std::string& id) {
    return texts[id];
}

std::vector<unsigned char> F_Resource::GetData(const std::string& id)
{
    return datas[id];
}

bool F_Resource::HasResource(const std::string& id)
{
    if (textures.count(id)!=0) {
        return 1;
    }
    else if (sounds.count(id) != 0) {
        return 1;
    }
    else if (fonts.count(id) != 0) {
        return 1;
    }
    else if (texts.count(id) != 0) {
        return 1;
    }
    else if (musics.count(id) != 0) {
        return 1;
    }
    else if (datas.count(id) != 0) {
        return 1;
    }
    return 0;

}

bool F_Resource::HasResource(const std::string& id, ResourceType type)
{
    switch (type) {
    case ResTexture:
        return textures.count(id) != 0;
    case ResSound:
        return sounds.count(id) != 0;
    case ResMusic:
        return musics.count(id) != 0;
    case ResFont:
        return fonts.count(id) != 0;
    case ResText:
        return texts.count(id) != 0;
    case ResData:
        return datas.count(id) != 0;
    default:
        return 0;
    }
}

// 获取资源指针
Texture2D* F_Resource::GetTextureP(const std::string& id) {
    return &textures[id];
}

Sound* F_Resource::GetSoundP(const std::string& id) {
    return &sounds[id];
}

Music* F_Resource::GetMusicP(const std::string& id) {
    return &musics[id];
}

Font* F_Resource::GetFontP(const std::string& id) {
    return &fonts[id];
}

std::string* F_Resource::GetTextP(const std::string& id) {
    return &texts[id];
}

// 卸载单个资源
void F_Resource::UnloadResource(const std::string& id, ResourceType type) {
    switch (type) {
    case ResTexture:
        UnloadTextureById(id);
        break;
    case ResSound:
        UnloadSoundById(id);
        break;
    case ResMusic:
        UnloadMusicById(id);
        break;
    case ResFont:
        UnloadFontById(id);
        break;
    case ResText:
        UnloadTextById(id);
        break;
    default:
        break;
    }
}

// 卸载所有资源
void F_Resource::UnloadAllResources() {
    for (auto& texture : textures) {
        UnloadTexture(texture.second);
    }
    textures.clear();

    for (auto& sound : sounds) {
        UnloadSound(sound.second);
    }
    sounds.clear();

    for (auto& music : musics) {
        UnloadMusicStream(music.second);
    }
    musics.clear();

    for (auto& font : fonts) {
        UnloadFont(font.second);
    }
    fonts.clear();

    texts.clear();
    for (auto& data : datas) {
		data.second.clear();
    }
	datas.clear();
}

// 资源加载辅助函数
bool F_Resource::LoadTextureFromZlib(const std::string& path, const std::string& id) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        Image image = LoadImageFromMemory(GetFileExtension(path.c_str()), data.data(), data.size());
        if (image.data != nullptr) {
            Texture2D texture = LoadTextureFromImage(image);
            UnloadImage(image);
            AddResource(id, ResTexture, &texture);
            return true;
        }
    }
    return false;
}

bool F_Resource::LoadSoundFromZlib(const std::string& path, const std::string& id) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        Wave wave = LoadWaveFromMemory(GetFileExtension(path.c_str()), data.data(), data.size());
        Sound sound = LoadSoundFromWave(wave);
        AddResource(id, ResSound, &sound);
        return true;
    }
    return false;
}

bool F_Resource::LoadMusicFromZlib(const std::string& path, const std::string& id) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        Music music = LoadMusicStreamFromMemory(GetFileExtension(path.c_str()), data.data(), data.size());
        AddResource(id, ResMusic, &music);
        return true;
    }
    return false;
}

bool F_Resource::LoadFontFromZlib(const std::string& path, const std::string& id, int font_size, int* codepoints, int codepoints_size) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        Font font = LoadFontFromMemory(GetFileExtension(path.c_str()), data.data(), data.size(), font_size, codepoints, codepoints_size);
        AddResource(id, ResFont, &font);
        return true;
    }
    return false;
}

bool F_Resource::LoadTextFromZlib(const std::string& path, const std::string& id) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        std::string text(data.begin(), data.end());
        AddResource(id, ResText, &text);
        return true;
    }
    return false;
}

bool F_Resource::LoadDataFromZlib(const std::string& path, const std::string& id)
{
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        AddResource(id, ResData, &data);
        return true;
    }
    return false;
}

// 卸载资源辅助函数
void F_Resource::UnloadTextureById(const std::string& id) {
    if (textures.find(id) != textures.end()) {
        UnloadTexture(textures[id]);
        textures.erase(id);
    }
}

void F_Resource::UnloadSoundById(const std::string& id) {
    if (sounds.find(id) != sounds.end()) {
        UnloadSound(sounds[id]);
        sounds.erase(id);
    }
}

void F_Resource::UnloadMusicById(const std::string& id) {
    if (musics.find(id) != musics.end()) {
        UnloadMusicStream(musics[id]);
        musics.erase(id);
    }
}

void F_Resource::UnloadFontById(const std::string& id) {
    if (fonts.find(id) != fonts.end()) {
        UnloadFont(fonts[id]);
        fonts.erase(id);
    }
}

void F_Resource::UnloadTextById(const std::string& id) {
    texts.erase(id);
}

void F_Resource::UnloadDataById(const std::string& id)
{
	datas.erase(id);
}

// 解压缩辅助函数
bool F_Resource::DecompressZlib(const std::string& path, std::vector<unsigned char>& out_data) {
    return DecompressZlibWithPassword(path, out_data, _password);
}

bool F_Resource::DecompressZlibWithPassword(const std::string& path, std::vector<unsigned char>& out_data, const char* password) {
    unzFile zipfile = nullptr;
    if (_use_memory_zip) {
        // 从内存打开 ZIP
        zipfile = unzOpen2_64("in_memory.zip", &_mem_zip_io);
    }
    else {
        // 从文件路径打开 ZIP
        zipfile = unzOpen(_zip_path.c_str());
    }
    if (!zipfile) {
        DEBUG_LOG(LOG_ERROR,
            "F_Resource:打开压缩文件失败", 0);
        return false;
    }
    if (unzLocateFile(zipfile, path.c_str(), 0) != UNZ_OK) {
        DEBUG_LOG(LOG_ERROR,
            "F_Resource:在压缩文件中定位文件失败", 0);
        unzClose(zipfile);
        return false;
    }
    if (has_password) {
        if (unzOpenCurrentFilePassword(zipfile, password) != UNZ_OK) {
            DEBUG_LOG(LOG_ERROR,
                "F_Resource:使用密码在压缩文件中打开文件失败", 0);
            unzClose(zipfile);
            return false;
        }
    }
    else if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
        DEBUG_LOG(LOG_ERROR,
            "F_Resource:不使用密码在压缩文件中打开文件失败", 0);
        unzClose(zipfile);
        return false;
    }
    DEBUG_LOG(LOG_INFO,
     "F_Resource:成功打开文件在压缩文件中", 0);

    unz_file_info file_info;
    if (unzGetCurrentFileInfo(zipfile, &file_info, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {

        DEBUG_LOG(LOG_WARNING,
            "F_Resource:无法获取压缩文件信息", 0); unzCloseCurrentFile(zipfile);
        unzClose(zipfile);
        return false;
    }

    

    out_data.resize(file_info.uncompressed_size);
    int result = unzReadCurrentFile(zipfile, out_data.data(), file_info.uncompressed_size);
  
    if (result != file_info.uncompressed_size) {
        DEBUG_LOG(LOG_WARNING,
            "F_Resource:文件解压缩数据不完整或失败", 0);
        unzCloseCurrentFile(zipfile);
        unzClose(zipfile);
        return false;
    }
    else if (result < 0) {
        DEBUG_LOG(LOG_WARNING,
            "F_Resource:unzReadCurrentFile没有数据", 0);
    }

    unzCloseCurrentFile(zipfile);
    unzClose(zipfile);

    return result == file_info.uncompressed_size;
}
