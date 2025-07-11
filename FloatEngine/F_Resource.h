/**
 * @file F_Resource.h
 * @brief ��Դ������
 * @defgroup Resource ��Դģ��
 *
 * �ṩ��Դ�ļ��ء������ж�ع��ܣ�
 * - ֧�ֶ�����Դ���ͣ��ı������������֡��������塢���ݣ�
 * - ֧�ִ�ѹ�����м�����Դ
 * - ֧����Դ�Ļ����ָ���ȡ
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
  * @brief ��Դ����ö��
  */
enum ResourceType {
    ResText,    ///< �ı���Դ
    ResSound,   ///< ������Դ
    ResMusic,   ///< ������Դ
    ResTexture, ///< ������Դ
    ResFont,    ///< ������Դ
    ResData     ///< ������Դ
};

typedef struct {
    unsigned char* buffer; ///< �ڴ滺����ָ��
    size_t size;           ///< ��������ǰ��С
    size_t capacity;       ///< ������������
    size_t position;       ///< ��ǰ��дλ��
} MemoryBuffer;

/**
 * @class F_Resource
 * @ingroup Resource
 *
 * @brief �ṩ��Դ�ļ��ء������ж�ع���
 *
 * @note ʹ��ʾ����
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
    std::vector<unsigned char> _zip_data;  // �洢�ڴ��е� ZIP ����
    bool _use_memory_zip = false;          // ����Ƿ�ʹ���ڴ� ZIP
public:
    /**
     * @brief ���캯��
     */
    F_Resource();

    /**
     * @brief ��������
     */
    ~F_Resource();

    /**
     * @brief ����zip·��
     * @param path zip�ļ�·��
     */
    void SetZipPath(const std::string& path);

    /**
     * @brief ����zip�ļ�Data
     * @param data zip�ļ�����
     */
    void SetZipData(std::vector<unsigned char> data);

    /**
     * @brief ����ѹ��������
     * @param password ����
     */
    void SetPassword(const char* password);

    /**
     * @brief ��ʹ������
     */
    void NoPassword();

    /**
     * @brief ����ѹ����
     * @param path ѹ����·��
     * @param password ����
     * @param _has_password �Ƿ�������
     */
    inline void Setup(const std::string& path, const char* password = "", bool _has_password = 0) {
        SetZipPath(path);
        SetPassword(password);
        has_password = _has_password;
    }
    /**
     * @brief ����Դ�����ļ��ж�ȡ��Դ(.json)
     * @param path ��Դ�����ļ�
     * @return �ɹ�����true�����򷵻�false
     */
    bool LoadResources(const std::string& path);
    /**
     * @brief ����Դ�����ļ��ж�ȡ����(.json)
     * @param path ������Դ�����ļ�
     * @return �ɹ�����true�����򷵻�false
     */
    bool LoadFontResourceFromConfigFile(const std::string& path);

    /**
     * @brief ������Դ
     * @param path ��Դ·��
     * @param id ��ԴID
     * @param type ��Դ����
     * @return �ɹ�����true�����򷵻�false
     */
    bool LoadResource(const std::string& path, const std::string& id, ResourceType type);
    /**
     * @brief ����������Դ(��ʱ��idΪ�ļ���)
	 * @param mode ����ģʽ(0:��Ƶ����Ϊsound����,1:��Ƶ����Ϊmusic����,2:��Ƶ�ļ�ͬʱΪmusic��sound����)
     */
    bool LoadAllResource(int mode);
    /**
     * @brief ����id�õ�path 
    */
    std::string GetResPath(const std::string& id);
   /**
     * @brief ����������Դ
     * @param path ����·��
     * @param id ��ԴID
     * @param font_size �����С
     * @param codepoints �������
     * @param codepoints_size ��������С
     * @return �ɹ�����true�����򷵻�false
     */
    bool LoadFontResource(const std::string& path, const std::string& id, int font_size, int* codepoints, int codepoints_size);
   
    /**
     * @brief ������Դ
     * @param id ��ԴID
     * @param type ��Դ����
     * @param resource ��Դָ��
     */
    void AddResource(const std::string& id, ResourceType type, void* resource);

    /**
     * @brief ��ȡ������Դ
     * @param id ��ԴID
     * @return ������Դ
     */
    Texture2D GetTexture(const std::string& id);

    /**
     * @brief ��ȡ������Դ
     * @param id ��ԴID
     * @return ������Դ
     */
    Sound GetSound(const std::string& id);

    /**
     * @brief ��ȡ������Դ
     * @param id ��ԴID
     * @return ������Դ
     */
    Music GetMusic(const std::string& id);

    /**
     * @brief ��ȡ������Դ
     * @param id ��ԴID
     * @return ������Դ
     */
    Font GetFont(const std::string& id);

    /**
     * @brief ��ȡ�ı���Դ
     * @param id ��ԴID
     * @return �ı���Դ
     */
    std::string GetText(const std::string& id);

    /**
     * @brief ��ȡ������Դ
     * @param id ��ԴID
     * @return ������Դ
     */
    std::vector<unsigned char> GetData(const std::string& id);

    /**
     * @brief �ж���Դ�Ƿ����
     * @param id ��ԴID
     * @return ���ڷ���true�����򷵻�false
     */
    bool HasResource(const std::string& id);

    /**
     * @brief �ж���Դ�Ƿ����
     * @param id ��ԴID
     * @param type ��Դ����
     * @return ���ڷ���true�����򷵻�false
     */
    bool HasResource(const std::string& id, ResourceType type);

    /**
     * @brief ��ȡ������Դָ��
     * @param id ��ԴID
     * @return ������Դָ��
     */
    Texture2D* GetTextureP(const std::string& id);

    /**
     * @brief ��ȡ������Դָ��
     * @param id ��ԴID
     * @return ������Դָ��
     */
    Sound* GetSoundP(const std::string& id);

    /**
     * @brief ��ȡ������Դָ��
     * @param id ��ԴID
     * @return ������Դָ��
     */
    Music* GetMusicP(const std::string& id);

    /**
     * @brief ��ȡ������Դָ��
     * @param id ��ԴID
     * @return ������Դָ��
     */
    Font* GetFontP(const std::string& id);

    /**
     * @brief ��ȡ�ı���Դָ��
     * @param id ��ԴID
     * @return �ı���Դָ��
     */
    std::string* GetTextP(const std::string& id);

    /**
     * @brief ж�ص�����Դ
     * @param id ��ԴID
     * @param type ��Դ����
     */
    void UnloadResource(const std::string& id, ResourceType type);

    /**
     * @brief ж��������Դ
     */
    void UnloadAllResources();
    /**
     * @brief ��ȡTexture����Դ����
    */
	std::map<std::string, Texture> GetTextureCache() {return textures;}
	/**
	 * @brief ��ȡSound����Դ����
	*/
	std::map<std::string, Sound> GetSoundCache() { return sounds; }
	/**
	 * @brief ��ȡMusic����Դ����
	*/
	std::map<std::string, Music> GetMusicCache() { return musics; }

	/**
	 * @brief ��ȡFont����Դ����
	*/
	std::map<std::string, Font> GetFontCache() { return fonts; }
	/**
	 * @brief ��ȡText����Դ����
	*/
	std::map<std::string, std::string> GetTextCache() { return texts; }
	/**
	 * @brief ��ȡData����Դ����
	*/
	std::map<std::string, std::vector<unsigned char>> GetDataCache() { return datas; }
	/**
	 * @brief ��ȡѹ��������
	 * @return ѹ��������
	 */
	std::vector<unsigned char> GetZipData() { return _zip_data; }
	/**
	 * @brief ��ȡѹ����·��
	 * @return ѹ����·��
	 */
	std::string GetZipPath() { return _zip_path; }
private:
    // ��Դ����
    std::map<std::string, Texture2D> textures;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Music> musics;
    std::map<std::string, Font> fonts;
    std::map<std::string, std::string> texts;
    std::map<std::string, std::vector<unsigned char>> datas;
    //path����
	std::map<std::string, std::string> paths;
    // ��Դ���ظ�������
    bool LoadTextureFromZlib(const std::string& path, const std::string& id);
    bool LoadSoundFromZlib(const std::string& path, const std::string& id);
    bool LoadMusicFromZlib(const std::string& path, const std::string& id);
    bool LoadFontFromZlib(const std::string& path, const std::string& id, int font_size, int* codepoints, int codepoints_size);
    bool LoadTextFromZlib(const std::string& path, const std::string& id);
    bool LoadDataFromZlib(const std::string& path, const std::string& id);

    // ж����Դ��������
    void UnloadTextureById(const std::string& id);
    void UnloadSoundById(const std::string& id);
    void UnloadMusicById(const std::string& id);
    void UnloadFontById(const std::string& id);
    void UnloadTextById(const std::string& id);
    void UnloadDataById(const std::string& id);

    // ��ѹ����������
    bool DecompressZlib(const std::string& path, std::vector<unsigned char>& out_data);
    bool DecompressZlibWithPassword(const std::string& path, std::vector<unsigned char>& out_data, const char* password);
};

/**
 * @brief ���ı����ݱ��浽 ZIP �ļ���ָ��·��
 *
 * @param zipfile ZIP �ļ�·��
 * @param filename ZIP ���ļ�·��
 * @param text �ı�����
 * @return true ����ɹ�
 * @return false ����ʧ��
 */
bool SaveFileTextToZip(const char* zipfile,const char*filename, const char* text);

/***
 * @brief �����������ݱ��浽 ZIP �ļ���ָ��·��
 *
 * @param zipfile ZIP �ļ�·��
 * @param filename ZIP ���ļ�·��
 * @param data ����������
 * @param data_size ���ݴ�С
 * @return true ����ɹ�
 * @return false ����ʧ��
 */
bool SaveFileDataToZip(const char* zipfile, const char* filename, const unsigned char* data, int data_size);
#endif // F_RESOURCE_H
