/**
 * @file F_Font.h
 * @brief 字体管理类
 * @defgroup Font 字体模块
 *
 * 提供字体的加载和管理功能：
 * - 从文件加载字体
 * - 卸载字体
 */

#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "raylib.hpp"

 /**
  * @class F_Font
  * @ingroup Font
  *
  * @brief 提供字体的加载和管理功能
  *
  * @note 使用示例：
  * @code
  * F_Font font;
  * font.Load("font.ttf", "ttf", 32, nullptr, 0);
  * @endcode
  */
class F_Font {
    Font _rayfnt;
    unsigned char* _temp_data;
    char* _temp_path;
    std::vector<int> _codepoints;
public:
    /**
     * @brief 默认构造函数
     */
    F_Font();

    /**
     * @brief 拷贝构造函数
     * @param font F_Font对象
     */
    F_Font(const F_Font& font);

    /**
     * @brief 构造函数
     * @param _rfont Font对象
     */
    F_Font(const Font& _rfont);

    /**
     * @brief 转换为raylib的Font对象
     * @return Font对象
     */
    struct Font to_raylib_font();

    /**
     * @brief 从文件加载字体
     * @param filename 文件名
     * @param type 文件类型
     * @param font_size 字体大小
     * @param codepoints 初始码点数组
     * @param codepoints_count 初始码点数组大小
     */
    void Load(const char* filename, const char* type, int font_size, int* codepoints, int codepoints_count);

    /**
    * @brief 从数据中加载字体
	* @param data 字体数据
	* @param data_size 数据大小
    * @param type 字体类型
	* @param font_size 字体大小
    * @param codepoints 初始码点数组
    * @param codepoints_count 初始码点数组大小

   */
    void LoadFromData(const unsigned char* data, int data_size, const char* type, int font_size, int* codepoints, int codepoints_count);
    /**
     * @brief 卸载字体
     */
    void Unload();
	/**
	 * @brief 获取字体的码点数组
	 * @return 码点数组
	 */
    int* GetCoepoints() { return _codepoints.data(); }
    /**
	* @brief 添加码点并重新加载字体
    */
    void ReloadWithNewCodepoints(int* codepoints);
    /**
	* @brief 检测文本中的Unicode字符是否在字体的码点数组中,没有则加入
    */
	void CheckAndAddCodepoints(const std::string& text);

    /**
    * @brief 检查_codepoints是否包含文本
    */
    bool ContainsText(const std::string& text) const;
};

/**
 * @namespace floatapi_font
 * @ingroup Font
 *
 * @brief 提供字体相关的辅助函数
 */
namespace floatapi_font {
    /**
     * @brief 初始化默认字体
     */
    void InitDefaultFont();

    /**
     * @brief 从文件加载raylib字体
     * @param filename 文件名
     * @param type 文件类型
     * @param font_size 字体大小
     * @param codepoints 码点数组
     * @param codepoints_count 码点数组大小
     * @return 加载的Font对象
     */
    Font LoadFontRaylib(const char* filename, const char* type, int font_size, int* codepoints, int codepoints_count);

    /**
     * @brief 从数据中加载raylib字体
     * @param data 字体数据
     * @param data_size 数据大小
     * @param type 文件类型
     * @param font_size 字体大小
     * @param codepoints 码点数组
     * @param codepoints_count 码点数组大小
     * @return 加载的Font对象
     */
    Font LoadFontRaylibFromData(const unsigned char* data, int data_size, const char* type, int font_size, int* codepoints, int codepoints_count);

    /**
     * @brief 加载默认中文字体
     * @return 加载的F_Font对象
     */
    F_Font LoadChineseF_FontDefault();
    /**
     * @brief 加载默认中文字体
     * @return 加载的Font对象
     */
    Font LoadChineseFontDefault();

    /**
     * @brief 加载常用字符码点
     * @param count 码点数量
     * @param other 其他字符
     * @return 码点数组
     */
    int* LoadCommonCodepoints(int& count, std::string other = "");

    /**
     * @brief 根据范围加载码点
     * @param start 起始码点
     * @param end 结束码点
     * @param count 码点数量
     * @return 码点数组
     */
    int* LoadCodepointsFromRange(int start, int end, int& count);

    /**
     * @brief 加载中文字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组
     */
    int* LoadChineseCodepoints(int& count);

    /**
     * @brief 加载日文字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组
     */
    int* LoadJapaneseCodepoints(int& count);

    /**
     * @brief 加载法语字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组
     */
    int* LoadFrenchCodepoints(int& count);

    /**
     * @brief 加载德语字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组
     */
    int* LoadGermanCodepoints(int& count);

    /**
     * @brief 加载西班牙语字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组
     */
    int* LoadSpanishCodepoints(int& count);

    /**
     * @brief 加载ASCII字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组
     */
    int* LoadAsciiCodepoints(int& count);

    /**
     * @brief 加载Emoji字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组
     */
    int* LoadEmojiCodepoints(int& count);

    /**
     * @brief 判断是否是有效的Unicode字符
     * @param codepoint Unicode码点
     * @return 如果有效返回true，否则返回false
     */
    bool IsValidUnicodeCodepoint(int codepoint);

    /**
     * @brief 加载0到0xFFFF范围内所有有效的Unicode码点
     * @param count 码点数量
     * @param filter 是否过滤无效码点
     * @return 码点数组
     */
    int* LoadUnicodeCodepoints(int& count, bool filter = true);

    /**
     * @brief 拼接多个codepoints数组
     * @param codepoints1 码点数组1
     * @param count1 码点数量1
     * @param codepoints2 码点数组2
     * @param count2 码点数量2
     * @param count 拼接后的码点数量
     * @return 拼接后的码点数组
     */
    int* ConcatenateCodepoints(int* codepoints1, int count1, int* codepoints2, int count2, int& count);
}

