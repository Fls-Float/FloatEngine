#ifndef F_RESOURCE_H
#define F_RESOURCE_H

#include <raylib.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <set>
#include <utility>
// 资源类型枚举
enum ResourceType {
    ResTexture,
    ResSound,
    ResMusic,
    ResFont,
    ResText,
    ResData
};

class F_Resource {
public:
    F_Resource();
    ~F_Resource();

    void SetResourcePath(const std::string& path);
    void SetResourceData(const std::vector<unsigned char>& data);
    void SetPassword(const std::string& password);
    int GetFileCount();

    // 单资源加载
    bool LoadResource(const std::string& internalPath, const std::string& id, ResourceType type, bool lazyLoad = false);
    bool LoadFontResource(const std::string& internalPath, const std::string& id, int font_size, int* codepoints = nullptr, int codepoints_size = 0,bool lazyLoad=false);

    // 批量资源管理
    bool CreateLoadBatch(const std::string& batchId, const std::vector<std::pair<std::string, ResourceType>>& resources);
    bool LoadResourceToBatch(const std::string& internalPath, const std::string& id, ResourceType type, const std::string& batchId);
    bool LoadFontResourceToBatch(const std::string& internalPath, const std::string& id, int font_size, const std::string& batchId, int* codepoints = nullptr, int codepoints_size = 0);
	void LoadBatchResources(const std::string& batchId);
	void UnloadBatchResources(const std::string& batchId);

    // 获取资源
    Texture2D GetTexture(const std::string& id);
    Sound GetSound(const std::string& id);
    Music GetMusic(const std::string& id);
    Font GetFont(const std::string& id);
    std::string GetText(const std::string& id);
    std::vector<unsigned char> GetData(const std::string& id);

    bool HasResource(const std::string& id, ResourceType type) const;


    // 卸载资源
    void UnloadResource(const std::string& id, ResourceType type);
    void UnloadAllResources();

    std::string GetResourcePath(const std::string& id);

private:
    std::string resource_path;
    std::vector<unsigned char> resource_data;
    bool use_memory_zip;
    std::string zip_password;
    // 资源缓存
    std::map<std::string, Texture2D> textures;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Music> musics;
    std::map<std::string, Font> fonts;
    std::map<std::string, std::string> texts;
    std::map<std::string, std::vector<unsigned char>> datas;
    std::map<std::string, std::string> paths;

    // 懒加载索引 & 批量区块
    std::set<std::pair<std::string, ResourceType>> lazyLoadedResources;
    std::map<std::string, std::set<std::pair<std::string, ResourceType>>> resourceBatches;

    bool LoadResourceInternal(const std::string& internalPath, const std::string& id, ResourceType type, int font_size = 0, int* codepoints = nullptr, int codepoints_size = 0);
    std::vector<unsigned char> ExtractFileFromZip(const std::string& internalPath);
    bool IsValidZipFile() const;
    std::string DetectFileFormat(const std::vector<unsigned char>& data);

    void UnloadTextureById(const std::string& id);
    void UnloadSoundById(const std::string& id);
    void UnloadMusicById(const std::string& id);
    void UnloadFontById(const std::string& id);
    void UnloadTextById(const std::string& id);
    void UnloadDataById(const std::string& id);
    
};

#endif // F_RESOURCE_H
