#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <algorithm>
#include <atomic>           
#include <chrono>

// 资源类型枚举
enum ResourceType {
    ResTexture,
    ResSound,
    ResMusic,
    ResFont,
    ResText,
    ResData,
    ResUnknown
};

// ----------------------------------------------------------------------
// 资源状态结构体 (内存和引用计数)
// ----------------------------------------------------------------------
struct ResourceState {
    int ref_count = 0;
    size_t memory_usage_bytes = 0;
    std::chrono::steady_clock::time_point last_access_time;
    ResourceType type = ResTexture;
};

// ----------------------------------------------------------------------
// 资源元数据结构体 (加载参数，用于懒加载和批次)
// ----------------------------------------------------------------------
struct ResourceMetadata {
    ResourceType type = ResTexture;
    std::string internalPath;

    // 字体专属参数
    int font_size = 0;
    std::vector<int> codepoints;
};

// ----------------------------------------------------------------------
// 模拟异步任务结构体 (仅用于统一 UploadResourceToGPU 的接口)
// ----------------------------------------------------------------------
struct LoadTask {
    std::string id;
    ResourceType type;
    std::vector<unsigned char> raw_data;
    int font_size = 0;
    std::vector<int> codepoints;
};


class F_Resource {
public:
    F_Resource();
    ~F_Resource();

    // 配置函数
    void SetResourcePath(const std::string& path);
    void SetResourceData(const std::vector<unsigned char>& data);
    void SetPassword(const std::string& password);
    void SetMemoryBudget(size_t maxBytes);
    void SetEnableMemoryMonitoring(bool enable); // 保留内存监控开关

    // 单资源加载
    bool LoadResource(const std::string& internalPath, const std::string& id, ResourceType type, bool lazyLoad = false);
    bool LoadFontResource(const std::string& internalPath, const std::string& id, int font_size, const std::vector<int>& codepoints = {}, bool lazyLoad = false);

    // 批量资源管理
    bool CreateLoadBatch(const std::string& batchId, const std::vector<std::pair<std::string, ResourceType>>& resources);
    bool LoadResourceToBatch(const std::string& internalPath, const std::string& id, ResourceType type, const std::string& batchId);
    bool LoadFontResourceToBatch(const std::string& internalPath, const std::string& id, int font_size, const std::string& batchId, const std::vector<int>& codepoints = {});
    void LoadBatchResources(const std::string& batchId);
    void UnloadBatchResources(const std::string& batchId);

    // 获取/状态
    Texture2D GetTexture(const std::string& id);
    Sound GetSound(const std::string& id);
    Music GetMusic(const std::string& id);
    Font GetFont(const std::string& id);
    std::string GetText(const std::string& id);
    std::vector<unsigned char> GetData(const std::string& id);
    bool HasResource(const std::string& id, ResourceType type) const;
    ResourceType GetResourceType(const std::string& id);
    std::string GetResourcePath(const std::string& id);
    int GetFileCount();

    // 卸载
    void UnloadResource(const std::string& id, ResourceType type);
    void UnloadAllResources();

private:
    std::string resource_path;
    std::vector<unsigned char> resource_data;
    bool use_memory_zip;
    std::string zip_password;

    // 配置开关
    bool m_enableMemoryMonitoring = true;

    // 资源缓存
    std::map<std::string, Texture2D> textures;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Music> musics;
    std::map<std::string, Font> fonts;
    std::map<std::string, std::string> texts;
    std::map<std::string, std::vector<unsigned char>> datas;

    // 状态与批次
    std::map<std::string, ResourceMetadata> lazyLoadedResources;
    std::map<std::string, std::map<std::string, ResourceMetadata>> resourceBatches;
    std::map<std::string, ResourceState> resourceStates;

    // 内存追踪
    size_t m_maxMemoryBudget = 1024 * 1024 * 512;
    std::atomic<size_t> m_currentMemoryUsage{ 0 };

    // 内部函数 (同步)
    bool LoadResourceInternal(const std::string& internalPath, const std::string& id, ResourceType type, int font_size = 0, const std::vector<int>& codepoints = {});
    bool UploadResourceToGPU(const LoadTask& task);

    void CheckAndUnloadLRU(size_t requiredBytes);

    // Minizip-ng/Raylib Helpers
    std::vector<unsigned char> ExtractFileFromZip(const std::string& internalPath);
    std::string DetectFileFormat(const std::vector<unsigned char>& data);
    bool IsValidZipFile() const;

    // 内部卸载
    void UnloadTextureById(const std::string& id);
    void UnloadSoundById(const std::string& id);
    void UnloadMusicById(const std::string& id);
    void UnloadFontById(const std::string& id);
    void UnloadTextById(const std::string& id);
    void UnloadDataById(const std::string& id);

    void ExecuteUnload(const std::string& id, ResourceType type);
};