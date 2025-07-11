/**
 * @file F_Resource.h
 * @brief 资源管理类
 * @defgroup Resource 资源模块
 *
 * 提供资源的加载、管理和卸载功能：
 * - 支持多种资源类型（文本、声音、音乐、纹理、字体、数据）
 * - 支持从压缩包中加载资源
 * - 支持资源的缓存和指针获取
 */

#ifndef F_RESOURCE_H
#define F_RESOURCE_H

#include <raylib.hpp>
#include <map>
#include <string>
#include <vector>
#include <future>

 /**
  * @enum ResourceType
  * @ingroup Resource
  *
  * @brief 资源类型枚举
  */
enum ResourceType {
    ResText,    ///< 文本资源
    ResSound,   ///< 声音资源
    ResMusic,   ///< 音乐资源
    ResTexture, ///< 纹理资源
    ResFont,    ///< 字体资源
    ResData     ///< 数据资源
};

typedef struct {
    unsigned char* buffer; ///< 内存缓冲区指针
    size_t size;           ///< 缓冲区当前大小
    size_t capacity;       ///< 缓冲区总容量
    size_t position;       ///< 当前读写位置
} MemoryBuffer;

/**
 * @class F_Resource
 * @ingroup Resource
 *
 * @brief 提供资源的加载、管理和卸载功能
 *
 * @note 使用示例：
 * @code
 * F_Resource resource;
 * resource.SetZipPath("resources.zip");
 * resource.SetPassword("password");
 * resource.LoadResource("image.png", "image", ResTexture);
 * Texture2D texture = resource.GetTexture("image");
 * @endcode
 */
class F_Resource {
    const char* _password;
    std::string _zip_path;
    bool has_password;
    std::vector<unsigned char> _zip_data;  // 存储内存中的 ZIP 数据
    bool _use_memory_zip = false;          // 标记是否使用内存 ZIP
public:
    /**
     * @brief 构造函数
     */
    F_Resource();

    /**
     * @brief 析构函数
     */
    ~F_Resource();

    /**
     * @brief 设置zip路径
     * @param path zip文件路径
     */
    void SetZipPath(const std::string& path);

    /**
     * @brief 设置zip文件Data
     * @param data zip文件数据
     */
    void SetZipData(std::vector<unsigned char> data);

    /**
     * @brief 设置压缩包密码
     * @param password 密码
     */
    void SetPassword(const char* password);

    /**
     * @brief 不使用密码
     */
    void NoPassword();

    /**
     * @brief 设置压缩包
     * @param path 压缩包路径
     * @param password 密码
     * @param _has_password 是否有密码
     */
    inline void Setup(const std::string& path, const char* password = "", bool _has_password = 0) {
        SetZipPath(path);
        SetPassword(password);
        has_password = _has_password;
    }
    /**
     * @brief 从资源配置文件中读取资源(.json)
     * @param path 资源配置文件
     * @return 成功返回true，否则返回false
     */
    bool LoadResources(const std::string& path);
    /**
     * @brief 从资源配置文件中读取字体(.json)
     * @param path 字体资源配置文件
     * @return 成功返回true，否则返回false
     */
    bool LoadFontResourceFromConfigFile(const std::string& path);

    /**
     * @brief 加载资源
     * @param path 资源路径
     * @param id 资源ID
     * @param type 资源类型
     * @return 成功返回true，否则返回false
     */
    bool LoadResource(const std::string& path, const std::string& id, ResourceType type);
    /**
     * @brief 加载所有资源(此时的id为文件名)
	 * @param mode 加载模式(0:音频加载为sound优先,1:音频加载为music优先,2:音频文件同时为music和sound加载)
     */
    bool LoadAllResource(int mode);
    /**
     * @brief 根据id得到path 
    */
    std::string GetResPath(const std::string& id);
   /**
     * @brief 加载字体资源
     * @param path 字体路径
     * @param id 资源ID
     * @param font_size 字体大小
     * @param codepoints 码点数组
     * @param codepoints_size 码点数组大小
     * @return 成功返回true，否则返回false
     */
    bool LoadFontResource(const std::string& path, const std::string& id, int font_size, int* codepoints, int codepoints_size);
   
    /**
     * @brief 加入资源
     * @param id 资源ID
     * @param type 资源类型
     * @param resource 资源指针
     */
    void AddResource(const std::string& id, ResourceType type, void* resource);

    /**
     * @brief 获取纹理资源
     * @param id 资源ID
     * @return 纹理资源
     */
    Texture2D GetTexture(const std::string& id);

    /**
     * @brief 获取声音资源
     * @param id 资源ID
     * @return 声音资源
     */
    Sound GetSound(const std::string& id);

    /**
     * @brief 获取音乐资源
     * @param id 资源ID
     * @return 音乐资源
     */
    Music GetMusic(const std::string& id);

    /**
     * @brief 获取字体资源
     * @param id 资源ID
     * @return 字体资源
     */
    Font GetFont(const std::string& id);

    /**
     * @brief 获取文本资源
     * @param id 资源ID
     * @return 文本资源
     */
    std::string GetText(const std::string& id);

    /**
     * @brief 获取数据资源
     * @param id 资源ID
     * @return 数据资源
     */
    std::vector<unsigned char> GetData(const std::string& id);

    /**
     * @brief 判断资源是否存在
     * @param id 资源ID
     * @return 存在返回true，否则返回false
     */
    bool HasResource(const std::string& id);

    /**
     * @brief 判断资源是否存在
     * @param id 资源ID
     * @param type 资源类型
     * @return 存在返回true，否则返回false
     */
    bool HasResource(const std::string& id, ResourceType type);

    /**
     * @brief 获取纹理资源指针
     * @param id 资源ID
     * @return 纹理资源指针
     */
    Texture2D* GetTextureP(const std::string& id);

    /**
     * @brief 获取声音资源指针
     * @param id 资源ID
     * @return 声音资源指针
     */
    Sound* GetSoundP(const std::string& id);

    /**
     * @brief 获取音乐资源指针
     * @param id 资源ID
     * @return 音乐资源指针
     */
    Music* GetMusicP(const std::string& id);

    /**
     * @brief 获取字体资源指针
     * @param id 资源ID
     * @return 字体资源指针
     */
    Font* GetFontP(const std::string& id);

    /**
     * @brief 获取文本资源指针
     * @param id 资源ID
     * @return 文本资源指针
     */
    std::string* GetTextP(const std::string& id);

    /**
     * @brief 卸载单个资源
     * @param id 资源ID
     * @param type 资源类型
     */
    void UnloadResource(const std::string& id, ResourceType type);

    /**
     * @brief 卸载所有资源
     */
    void UnloadAllResources();
    /**
     * @brief 获取Texture的资源缓存
    */
	std::map<std::string, Texture> GetTextureCache() {return textures;}
	/**
	 * @brief 获取Sound的资源缓存
	*/
	std::map<std::string, Sound> GetSoundCache() { return sounds; }
	/**
	 * @brief 获取Music的资源缓存
	*/
	std::map<std::string, Music> GetMusicCache() { return musics; }

	/**
	 * @brief 获取Font的资源缓存
	*/
	std::map<std::string, Font> GetFontCache() { return fonts; }
	/**
	 * @brief 获取Text的资源缓存
	*/
	std::map<std::string, std::string> GetTextCache() { return texts; }
	/**
	 * @brief 获取Data的资源缓存
	*/
	std::map<std::string, std::vector<unsigned char>> GetDataCache() { return datas; }
	/**
	 * @brief 获取压缩包数据
	 * @return 压缩包数据
	 */
	std::vector<unsigned char> GetZipData() { return _zip_data; }
	/**
	 * @brief 获取压缩包路径
	 * @return 压缩包路径
	 */
	std::string GetZipPath() { return _zip_path; }
private:
    // 资源缓存
    std::map<std::string, Texture2D> textures;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Music> musics;
    std::map<std::string, Font> fonts;
    std::map<std::string, std::string> texts;
    std::map<std::string, std::vector<unsigned char>> datas;
    //path缓存
	std::map<std::string, std::string> paths;
    // 资源加载辅助函数
    bool LoadTextureFromZlib(const std::string& path, const std::string& id);
    bool LoadSoundFromZlib(const std::string& path, const std::string& id);
    bool LoadMusicFromZlib(const std::string& path, const std::string& id);
    bool LoadFontFromZlib(const std::string& path, const std::string& id, int font_size, int* codepoints, int codepoints_size);
    bool LoadTextFromZlib(const std::string& path, const std::string& id);
    bool LoadDataFromZlib(const std::string& path, const std::string& id);

    // 卸载资源辅助函数
    void UnloadTextureById(const std::string& id);
    void UnloadSoundById(const std::string& id);
    void UnloadMusicById(const std::string& id);
    void UnloadFontById(const std::string& id);
    void UnloadTextById(const std::string& id);
    void UnloadDataById(const std::string& id);

    // 解压缩辅助函数
    bool DecompressZlib(const std::string& path, std::vector<unsigned char>& out_data);
    bool DecompressZlibWithPassword(const std::string& path, std::vector<unsigned char>& out_data, const char* password);
};

/**
 * @brief 将文本内容保存到 ZIP 文件的指定路径
 *
 * @param zipfile ZIP 文件路径
 * @param filename ZIP 内文件路径
 * @param text 文本内容
 * @return true 保存成功
 * @return false 保存失败
 */
bool SaveFileTextToZip(const char* zipfile,const char*filename, const char* text);

/***
 * @brief 将二进制数据保存到 ZIP 文件的指定路径
 *
 * @param zipfile ZIP 文件路径
 * @param filename ZIP 内文件路径
 * @param data 二进制数据
 * @param data_size 数据大小
 * @return true 保存成功
 * @return false 保存失败
 */
bool SaveFileDataToZip(const char* zipfile, const char* filename, const unsigned char* data, int data_size);
#endif // F_RESOURCE_H
