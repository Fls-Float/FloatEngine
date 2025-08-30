#include "F_Resource.h"
#include "minizip/unzip.h"
#include "minizip/zip.h"
#include <zlib.h>
#include <iostream>
#include <fstream>
#include <direct.h>
#include "FloatApi.h"
#include <filesystem>
extern void DEBUG_LOG(int lv, const char* text, bool english, bool auto_enter,bool );


zlib_filefunc64_def _mem_zip_io = { 0 }; // �ڴ� ZIP �� IO �ص�

static unsigned long ZCALLBACK mem_read(void* opaque, void* buf, unsigned long size) {
    MemoryBuffer* mem = (MemoryBuffer*)opaque;
    size_t bytes_to_read = (mem->position + size <= mem->size) ? size : mem->size - mem->position;
    if (bytes_to_read > 0) {
        memcpy(buf, mem->buffer + mem->position, bytes_to_read);
        mem->position += bytes_to_read;
    }
    return (unsigned long)bytes_to_read;
}

// �Զ��嶨λ����
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

// ���캯��
F_Resource::F_Resource() : _password(nullptr) {
    has_password = 0;
}

// ��������
F_Resource::~F_Resource() {
    UnloadAllResources();
}

// ����zip·��
void F_Resource::SetZipPath(const std::string& path) {
    _use_memory_zip = false;  // �л����ļ�ģʽ
    _zip_path = path;
    // ����ļ��Ƿ����
    std::ifstream file(_zip_path);
    if (file.good()) {
        DEBUG_LOG(LOG_INFO,
            TextFormat("F_Resource.SetZipPath:ѹ���ļ�%s�ɹ���", path.c_str()), 0);
    }
    else {
        DEBUG_LOG(LOG_ERROR, "F_Resource.SetZipPath:�ļ�������", 0);
    }
}

void F_Resource::SetZipData(std::vector<unsigned char> data) {
    if (data.empty()) {
        DEBUG_LOG(LOG_WARNING, "����� ZIP ����Ϊ��", 0);
        return;
    }
    _zip_data = std::move(data);
    _use_memory_zip = true;  // ���ʹ���ڴ� ZIP

    // ��ʼ���ڴ� ZIP �� IO �ص�
    _mem_zip_io.zopen64_file = nullptr;
    _mem_zip_io.zread_file = (read_file_func)mem_read;
    _mem_zip_io.zwrite_file = nullptr;
    _mem_zip_io.ztell64_file = nullptr;
    _mem_zip_io.zseek64_file = (seek64_file_func)mem_seek;
    _mem_zip_io.zclose_file = nullptr;
    _mem_zip_io.zerror_file = nullptr;
    _mem_zip_io.opaque = &_zip_data;  // ���� ZIP ����ָ��
}

// ����ѹ��������
void F_Resource::SetPassword(const char* password) {
    _password = password;
    has_password = true;
}

void F_Resource::NoPassword()
{
    has_password = false;
}

namespace fs = std::filesystem;

bool F_Resource::LoadResources(const std::string& path)
{
    if (!FileExists(path.c_str())) {
        DEBUG_LOG(LOG_ERROR, "����һ: �� F_Resource::LoadResources �У�path ��Ч");
        return false;
    }

    // ��ȡ������ JSON
    std::string data = LoadFileText(path.c_str());
    F_Json::Json json;
    try {
        json = F_Json::Json::parse(data);
    }
    catch (const std::exception& e) {
        DEBUG_LOG(LOG_ERROR, "��������� F_Resource::LoadResources ��JSON ����ʧ��: %s", e.what());
        return false;
    }

    // ��ȡ����·����Ϣ
    fs::path original_path(path);
    fs::path base_dir = original_path.parent_path();  // ��ȡ JSON �ļ�����Ŀ¼

    // ��֤ info �ֶδ�����
    if (!json.has("info") || json["info"].isNull()) {
        DEBUG_LOG(LOG_ERROR, "���������� F_Resource::LoadResources ��JSON ȱ�� info ");
        return false;
    }
    auto info = json["info"];

    // ��ȡ��Դ·����ת��Ϊ����·��
    if (!info.has("res_path")) {
        DEBUG_LOG(LOG_ERROR, "������:�� F_Resource::LoadResources ��info ȱ�� res_path �ַ����ֶ�");
        return false;
    }

    fs::path res_relative(info["res_path"].asString());
    fs::path res_absolute = base_dir / res_relative;  // ·��ƴ�ӹؼ�����
    std::string res_path = res_absolute.make_preferred().string();  // ͳһ·����ʽ

    // ��֤��Դ·����Ч��
    if (!fs::exists(res_absolute)) {
        DEBUG_LOG(LOG_ERROR, "������: �� F_Resource::LoadResources ����Դ·����Ч - %s", res_path.c_str());
        return false;
    }
    SetZipPath(res_path);
    auto resources = json["resources"];
    for (int i = 0; i < resources.size(); i++) {
        auto res = resources[i];
        if (!res.has("name") || !res.has("type") || !res.has("path")){
            DEBUG_LOG(LOG_ERROR, "������: �� F_Resource::LoadResources �е�����Դʵ��ȱ�ٲ��� ");
            return false;
        }
        auto id = res["name"].asString();
        auto type = res["type"].asString();
        auto path = res["path"].asString();
        ResourceType type_value = ResText;
        if (type == "ResText")type_value = ResText;
        else if (type == "ResSound")type_value = ResSound;
        else if (type == "ResTexture")type_value = ResTexture;
        else if (type == "ResMusic")type = ResMusic;
        else if (type == "ResFont")type = ResFont;
        else if (type == "ResData")type = ResData;
        if (type_value == ResFont) {
            if (!res.has("font_config_path")) {
                DEBUG_LOG(LOG_ERROR, "������: ȱ����������·��");
                return false;
            }
            LoadFontResourceFromConfigFile(res["font_config_path"].asString());
        }
        else {
            LoadResource(path, id, type_value);
        }
    }
    return true;
}

bool F_Resource::LoadFontResourceFromConfigFile(const std::string& path) {
    // 1. ��ȡJSON�ļ�����
    std::string jsonContent = LoadFileText(path.c_str());

    // 2. ����JSON����
    F_Json::Json config;
    try {
        config = F_Json::Json::parse(jsonContent);
    } catch (...) {
        return false;
    }

    // 3. ��֤�����ṹ
    if (!config.has("font_path") || 
        !config.has("font_size") ||
        !config.has("charset_ranges")||
        !!config.has("res_id")) {
        return false;
    }

    // 4. ��ȡ��������
    std::string fontPath = config["font_path"].asString();
    std::string id = config["res_id"].asString();
    int fontSize = static_cast<int>(config["font_size"].asNumber());

    // 5. �����ַ�����б�
    std::vector<int> codepoints;
    auto ranges = config["charset_ranges"];
    
    if (ranges.type() != F_Json::JsonType::Array) {
        return false;
    }

    for (size_t i = 0; i < ranges.size(); ++i) {
        F_Json::Json range = ranges[i];
        if (!range.has("start") || !range.has("end")) {
            continue;
        }

        int start = static_cast<int>(range["start"].asNumber());
        int end = static_cast<int>(range["end"].asNumber());

        // ��ӷ�Χ������б�
        for (int code = start; code <= end; ++code) {
            codepoints.push_back(code);
        }
    }

    // 6. ��������
    if (codepoints.empty()) {
        return false; // û����Ч�ַ���Χ
    }
    LoadFontResource(fontPath, id, fontSize, codepoints.data(), codepoints.size());

    return true;
}

// ������Դ
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
        return LoadDataFromZlib(path, id);
    default:
        return false;
    }
}

bool F_Resource::LoadAllResource(int mode) {
    unzFile zipfile = nullptr;

    // �� ZIP �ļ�
    try {
        if (_use_memory_zip) {
            zipfile = unzOpen2_64("in_memory.zip", &_mem_zip_io);
        }
        else {
            zipfile = unzOpen(_zip_path.c_str());

        }
    }
    catch (...) {
		DEBUG_LOG(LOG_ERROR, "F_Resource::LoadAllResource:(unzOpen)�޷��� ZIP �ļ�", 0);
		return false;
    }
    if (!zipfile) {
        DEBUG_LOG(LOG_ERROR, "F_Resource::LoadAllResource: �޷��� ZIP �ļ�", 0);
        return false;
    }
    // ���� ZIP �ļ��е�������Ŀ
    if (unzGoToFirstFile(zipfile) != UNZ_OK) {
        DEBUG_LOG(LOG_ERROR, "F_Resource::LoadAllResource: �޷���λ�� ZIP �ļ��ĵ�һ����Ŀ", 0);
        unzClose(zipfile);
        return false;
    }

    do {
        char filename[256];
        unz_file_info file_info;

        // ��ȡ��ǰ�ļ�����Ϣ
        if (0!=unzGetCurrentFileInfo(zipfile, &file_info, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK) {
            DEBUG_LOG(LOG_WARNING, "F_Resource::LoadAllResource: �޷���ȡ ZIP �ļ��е��ļ���Ϣ", 0);
            continue;
        }
        // �����ļ���չ���ж���Դ����
        std::string file_ext;
        if (GetFileExtension(filename) != nullptr)
            file_ext = GetFileExtension(filename);
        else file_ext = "";
        std::string file_id = GetFileName(filename); // ʹ���ļ�����Ϊ��Դ ID
        if (file_id == "" && file_ext == "") {
            continue;
        }
        DEBUG_LOG(LOG_DEBUG, TextFormat("ext=%s",file_ext.c_str()));
        if (file_ext == ".png" || file_ext == ".jpg" || file_ext == ".bmp" || file_ext=="fimg") {
            // ����������Դ
            DEBUG_LOG(LOG_DEBUG, file_ext.c_str());
            if (!LoadTextureFromZlib(filename, file_id)) {
                DEBUG_LOG(LOG_WARNING, TextFormat("F_Resource::LoadAllResource: �޷�����������Դ %s", filename), 0);
            }
        }
        else if (file_ext == ".wav" || file_ext == ".mp3" || file_ext == ".flac" || file_ext == ".ogg") {
            DEBUG_LOG(LOG_DEBUG, file_ext.c_str());
            if (mode == 0 || mode == 2) {
                if (!LoadSoundFromZlib(filename, file_id)) {
                    DEBUG_LOG(LOG_WARNING, TextFormat("F_Resource::LoadAllResource: �޷�����������Դ %s", filename), 0);
                }
            }
            if (mode == 1 || mode == 2) {
                if (!LoadMusicFromZlib(filename, file_id)) {
                    DEBUG_LOG(LOG_WARNING, TextFormat("F_Resource::LoadAllResource: �޷�����������Դ %s", filename), 0);
                }
            }
        }
        else if (file_ext == ".fsnd") {
            // ����������Դ
            DEBUG_LOG(LOG_DEBUG, file_ext.c_str());
            if (!LoadSoundFromZlib(filename, file_id)) {
                DEBUG_LOG(LOG_WARNING, TextFormat("F_Resource::LoadAllResource: �޷�����������Դ %s", filename), 0);
            }
        }
        else if (file_ext == ".fmus") {
            // ����������Դ
            DEBUG_LOG(LOG_DEBUG, file_ext.c_str());
            if (!LoadMusicFromZlib(filename, file_id)) {
                DEBUG_LOG(LOG_WARNING, TextFormat("F_Resource::LoadAllResource: �޷�����������Դ %s", filename), 0);
            }
        }
        else if (file_ext == ".ttf" || file_ext == ".otf"||file_ext==".ffnt") {
            // ����������Դ
            DEBUG_LOG(LOG_DEBUG, file_ext.c_str());
            if (!LoadFontFromZlib(filename, file_id, 24, nullptr, 0)) {
                DEBUG_LOG(LOG_WARNING, TextFormat("F_Resource::LoadAllResource: �޷�����������Դ %s", filename), 0);
            }
        }
        else {
            // �����ı���Դ
            if (!LoadTextFromZlib(filename, file_id)) {
                DEBUG_LOG(LOG_WARNING, TextFormat("F_Resource::LoadAllResource: �޷������ı���Դ %s", filename), 0);
            }
        }
    } while (unzGoToNextFile(zipfile) == UNZ_OK);

    // �ر� ZIP �ļ�
    unzClose(zipfile);
    return true;
}

std::string F_Resource::GetResPath(const std::string& id)
{
    return paths[id];
}


// ����������Դ
bool F_Resource::LoadFontResource(const std::string& path, const std::string& id, int font_size, int* codepoints, int codepoints_size) {
    return LoadFontFromZlib(path, id, font_size, codepoints, codepoints_size);
}


// ������Դ
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

// ��ȡ��Դ
Texture2D F_Resource::GetTexture(const std::string& id) {
    return textures[id];
}
Texture2D F_Resource::GetTexture(const std::string& id) const {
    return textures.at(id);
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

bool F_Resource::HasResource(const std::string& id, ResourceType type) const
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

// ��ȡ��Դָ��
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

// ж�ص�����Դ
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

// ж��������Դ
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

// ��Դ���ظ�������
bool F_Resource::LoadTextureFromZlib(const std::string& path, const std::string& id) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        paths[id] = path;
        Image image = LoadImageFromMemory(GetFileExtension(path.c_str()), data.data(), data.size());
        if (image.data != nullptr) {
            Texture2D texture = LoadTextureFromImage(image);
            UnloadImage(image);
            paths[id] = path;
            AddResource(id, ResTexture, &texture);
            return true;
        }
    }
    return false;
}

bool F_Resource::LoadSoundFromZlib(const std::string& path, const std::string& id) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        paths[id] = path;
        Wave wave = LoadWaveFromMemory(GetFileExtension(path.c_str()), data.data(), data.size());
        Sound sound = LoadSoundFromWave(wave);
        paths[id] = path;
        AddResource(id, ResSound, &sound);
        return true;
    }
    return false;
}

bool F_Resource::LoadMusicFromZlib(const std::string& path, const std::string& id) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        paths[id] = path;
        Music music = LoadMusicStreamFromMemory(GetFileExtension(path.c_str()), data.data(), data.size());
       
        AddResource(id, ResMusic, &music);
        return true;
    }
    return false;
}

bool F_Resource::LoadFontFromZlib(const std::string& path, const std::string& id, int font_size, int* codepoints, int codepoints_size) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        paths[id] = path;
        Font font = LoadFontFromMemory(GetFileExtension(path.c_str()), data.data(), data.size(), font_size, codepoints, codepoints_size);
        AddResource(id, ResFont, &font);
        return true;
    }
    return false;
}

bool F_Resource::LoadTextFromZlib(const std::string& path, const std::string& id) {
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        std::string text(reinterpret_cast<const char*>(data.data()), data.size());
        paths[id] = path;
        AddResource(id, ResText, &text);
        return true;
    }
    return false;
}

bool F_Resource::LoadDataFromZlib(const std::string& path, const std::string& id)
{
    std::vector<unsigned char> data;
    if (DecompressZlib(path, data)) {
        paths[id] = path;
        AddResource(id, ResData, &data);
        return true;
    }
    return false;
}

// ж����Դ��������
void F_Resource::UnloadTextureById(const std::string& id) {
    if (textures.find(id) != textures.end()) {
        paths[id] = "";
        UnloadTexture(textures[id]);
        textures.erase(id);
    }
}

void F_Resource::UnloadSoundById(const std::string& id) {
    if (sounds.find(id) != sounds.end()) {
        paths[id] = "";
        UnloadSound(sounds[id]);
        sounds.erase(id);
    }
}

void F_Resource::UnloadMusicById(const std::string& id) {
    if (musics.find(id) != musics.end()) {
        paths[id] = "";
        UnloadMusicStream(musics[id]);
        musics.erase(id);
    }
}

void F_Resource::UnloadFontById(const std::string& id) {
    if (fonts.find(id) != fonts.end()) {
        paths[id] = "";
        UnloadFont(fonts[id]);
        fonts.erase(id);
    }
}

void F_Resource::UnloadTextById(const std::string& id) {
    paths[id] = "";
    texts.erase(id);
}

void F_Resource::UnloadDataById(const std::string& id)
{
    paths[id] = "";
	datas.erase(id);
}

// ��ѹ����������
bool F_Resource::DecompressZlib(const std::string& path, std::vector<unsigned char>& out_data) {
    return DecompressZlibWithPassword(path, out_data, _password);
}

bool F_Resource::DecompressZlibWithPassword(const std::string& path, std::vector<unsigned char>& out_data, const char* password) {
    unzFile zipfile = nullptr;
    if (_use_memory_zip) {
        // ���ڴ�� ZIP
        zipfile = unzOpen2_64("in_memory.zip", &_mem_zip_io);
    }
    else {
        // ���ļ�·���� ZIP
        zipfile = unzOpen(_zip_path.c_str());
    }
    if (!zipfile) {
        DEBUG_LOG(LOG_ERROR,
            "F_Resource:��ѹ���ļ�ʧ��", 0);
        return false;
    }
    if (unzLocateFile(zipfile, path.c_str(), 0) != UNZ_OK) {
        DEBUG_LOG(LOG_ERROR,
           TextFormat( "F_Resource:��ѹ���ļ��ж�λ�ļ�ʧ��:%s",path.c_str()), 0);
        unzClose(zipfile);
        return false;
    }
    if (has_password) {
        if (unzOpenCurrentFilePassword(zipfile, password) != UNZ_OK) {
            DEBUG_LOG(LOG_ERROR,
             TextFormat(  "F_Resource:ʹ��������ѹ���ļ��д��ļ�ʧ��:%s", path.c_str()), 0);
            unzClose(zipfile);
            return false;
        }
    }
    else if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
        DEBUG_LOG(LOG_ERROR,
           TextFormat( "F_Resource:��ʹ��������ѹ���ļ��д��ļ�ʧ��:%s",path.c_str()), 0);
        unzClose(zipfile);
        return false;
    }
    DEBUG_LOG(LOG_INFO,
    TextFormat("F_Resource:�ɹ����ļ���ѹ���ļ���:%s", path.c_str()), 0);

    unz_file_info file_info;
    if (unzGetCurrentFileInfo(zipfile, &file_info, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {

        DEBUG_LOG(LOG_WARNING,
           TextFormat("F_Resource:�޷���ȡѹ���ļ���Ϣ:%s", path.c_str()), 0); unzCloseCurrentFile(zipfile);
        unzClose(zipfile);
        return false;
    }

    

    out_data.resize(file_info.uncompressed_size);
    int result = unzReadCurrentFile(zipfile, out_data.data(), file_info.uncompressed_size);
  
    if (result != file_info.uncompressed_size) {
        DEBUG_LOG(LOG_WARNING,
           TextFormat("F_Resource:�ļ���ѹ�����ݲ�������ʧ��:%s", path.c_str()), 0);
        unzCloseCurrentFile(zipfile);
        unzClose(zipfile);
        return false;
    }
    else if (result < 0) {
        DEBUG_LOG(LOG_WARNING,
            "F_Resource:unzReadCurrentFileû������", 0);
    }

    unzCloseCurrentFile(zipfile);
    unzClose(zipfile);

    return result == file_info.uncompressed_size;
}
#include "miniz.h"

bool SaveFileTextToZip(const char* zipfile, const char* filename, const char* text) {
    return SaveFileDataToZip(zipfile, filename,
        reinterpret_cast<const unsigned char*>(text),
        strlen(text) + 1); // +1 ���� null ��ֹ��
}

bool SaveFileDataToZip(const char* zipfile, const char* filename,
    const unsigned char* data, int data_size) {
    // 1. ���Զ�ȡ����ZIP�ļ�
    unsigned char* pExisting_zip_data = nullptr;
    size_t existing_zip_size = 0;
    mz_zip_archive source_archive;
    memset(&source_archive, 0, sizeof(source_archive));

    bool has_existing_zip = FileExists(zipfile);
    if (has_existing_zip) {
        // ��������ZIP�ļ�
        int fsize = 0;
        pExisting_zip_data = LoadFileData(zipfile, &fsize);
        existing_zip_size = static_cast<size_t>(fsize);

        // ��ʼ��ZIP�Ķ���
        if (!mz_zip_reader_init_mem(&source_archive, pExisting_zip_data, existing_zip_size, 0)) {
            if (pExisting_zip_data) UnloadFileData(pExisting_zip_data);
            has_existing_zip = false; // ���Ϊ��ЧZIP�ļ�
        }
    }

    // 2. ��ʼ��minizд����
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    if (!mz_zip_writer_init_heap(&zip_archive, 0, has_existing_zip ? 128 * 1024 : 0)) {
        if (has_existing_zip) {
            mz_zip_reader_end(&source_archive);
            UnloadFileData(pExisting_zip_data);
        }
        return false;
    }

    // 3. ��������ļ��������������Ч��
    if (has_existing_zip) {
        mz_uint num_files = mz_zip_reader_get_num_files(&source_archive);
        for (mz_uint i = 0; i < num_files; i++) {
            // ����ļ����Ƿ�ƥ��Ҫ��ӵ��ļ�
            char existing_filename[256];
            mz_zip_reader_get_filename(&source_archive, i, existing_filename, sizeof(existing_filename));

            // ����ͬ���ļ����������ļ��滻��
            if (strcmp(existing_filename, filename) == 0) continue;

            // ��������ļ�����ZIP
            if (!mz_zip_writer_add_from_zip_reader(&zip_archive, &source_archive, i)) {
                // ���ʧ�ܣ��������ļ�
                continue;
            }
        }

        // ����ZIP�Ķ���
        mz_zip_reader_end(&source_archive);
        UnloadFileData(pExisting_zip_data);
    }

    // 4. ������ļ�
    if (!mz_zip_writer_add_mem(&zip_archive, filename, data, data_size, MZ_BEST_COMPRESSION)) {
        mz_zip_writer_end(&zip_archive);
        return false;
    }

    // 5. ���д�벢��ȡZIP����
    void* pZip_data = nullptr;
    size_t zip_size = 0;
    if (!mz_zip_writer_finalize_heap_archive(&zip_archive, &pZip_data, &zip_size)) {
        mz_zip_writer_end(&zip_archive);
        return false;
    }

    // 6. ���浽�ļ�ϵͳ
    bool success = SaveFileData(zipfile,
       (pZip_data),
        static_cast<int>(zip_size));

    // 7. ������Դ
    mz_free(pZip_data);
    mz_zip_writer_end(&zip_archive);

    return success;
}