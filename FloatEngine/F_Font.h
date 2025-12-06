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
#include <memory>
#include "raylib.h"

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
    std::vector<unsigned char> _temp_data; // 使用vector管理内存
    std::string _temp_path;
    std::vector<int> _codepoints;
    int _font_size; // 记录字体大小
    std::string _font_type; // 记录字体类型

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
     * @brief 移动构造函数
     * @param font F_Font对象
     */
    F_Font(F_Font&& font) noexcept;

    /**
     * @brief 赋值运算符
     * @param font F_Font对象
     * @return F_Font&
     */
    F_Font& operator=(const F_Font& font);

    /**
     * @brief 移动赋值运算符
     * @param font F_Font对象
     * @return F_Font&
     */
    F_Font& operator=(F_Font&& font) noexcept;

    /**
     * @brief 构造函数
     * @param _rfont Font对象
     */
    explicit F_Font(const Font& _rfont);

    /**
     * @brief 析构函数
     */
    ~F_Font();

    /**
     * @brief 转换为raylib的Font对象
     * @return Font对象
     */
    Font to_raylib_font() const;

    /**
     * @brief 从文件加载字体
     * @param filename 文件名
     * @param type 文件类型
     * @param font_size 字体大小
     * @param codepoints 初始码点数组
     * @param codepoints_count 初始码点数组大小
     * @return 是否加载成功
     */
    bool Load(const char* filename, const char* type, int font_size, int* codepoints, int codepoints_count);

    /**
    * @brief 从数据中加载字体
    * @param data 字体数据
    * @param data_size 数据大小
    * @param type 字体类型
    * @param font_size 字体大小
    * @param codepoints 初始码点数组
    * @param codepoints_count 初始码点数组大小
    * @return 是否加载成功
    */
    bool LoadFromData(const unsigned char* data, int data_size, const char* type, int font_size, int* codepoints, int codepoints_count);

    /**
     * @brief 卸载字体
     */
    void Unload();

    /**
     * @brief 获取字体的码点数组
     * @return 码点数组
     */
    const std::vector<int>& GetCodepoints() const { return _codepoints; }

    /**
     * @brief 获取字体大小
     * @return 字体大小
     */
    int GetFontSize() const { return _font_size; }

    /**
     * @brief 获取字体类型
     * @return 字体类型
     */
    const std::string& GetFontType() const { return _font_type; }

    /**
     * @brief 添加码点并重新加载字体
     * @param codepoints 要添加的码点数组
     * @param count 码点数量
     * @return 是否成功重新加载
     */
    bool ReloadWithNewCodepoints(const std::vector<int>& new_codepoints);

    /**
     * @brief 检测文本中的Unicode字符是否在字体的码点数组中,没有则加入
     * @param text 要检测的文本
     * @return 是否检测到新字符并重新加载
     */
    bool CheckAndAddCodepoints(const std::string& text);

    /**
     * @brief 检查_codepoints是否包含文本
     * @param text 要检查的文本
     * @return 是否包含所有字符
     */
    bool ContainsText(const std::string& text) const;

    /**
     * @brief 重新加载字体（使用当前码点和参数）
     * @return 是否重新加载成功
     */
    bool ReloadFont();

    /**
     * @brief 检查字体是否有效
     * @return 字体是否有效
     */
    bool IsValid() const { return _rayfnt.texture.id != 0; }

private:
    /**
     * @brief 清理资源
     */
    void Cleanup();

    /**
     * @brief 从raylib字体中提取码点
     * @param font raylib字体对象
     */
    void ExtractCodepointsFromFont(const Font& font);
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
     * @return 加载的Font对象
     */
    Font LoadChineseFontDefault();

    /**
     * @brief 加载常用字符码点
     * @param count 码点数量
     * @param other 其他字符
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadCommonCodepoints(int& count, const std::string& other = "");

    /**
     * @brief 根据范围加载码点
     * @param start 起始码点
     * @param end 结束码点
     * @param count 码点数量
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadCodepointsFromRange(int start, int end, int& count);

    /**
     * @brief 加载中文字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadChineseCodepoints(int& count);

    /**
     * @brief 加载日文字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadJapaneseCodepoints(int& count);

    /**
     * @brief 加载法语字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadFrenchCodepoints(int& count);

    /**
     * @brief 加载德语字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadGermanCodepoints(int& count);

    /**
     * @brief 加载西班牙语字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadSpanishCodepoints(int& count);

    /**
     * @brief 加载ASCII字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadAsciiCodepoints(int& count);

    /**
     * @brief 加载Emoji字符集的所有Unicode码点
     * @param count 码点数量
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadEmojiCodepoints(int& count);

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
     * @return 码点数组（需要调用者释放）
     */
    std::vector<int> LoadUnicodeCodepoints(int& count, bool filter = true);

    /**
     * @brief 拼接多个codepoints数组
     * @param codepoints1 码点数组1
     * @param count1 码点数量1
     * @param codepoints2 码点数组2
     * @param count2 码点数量2
     * @param count 拼接后的码点数量
     * @return 拼接后的码点数组（需要调用者释放）
     */
    std::vector<int> ConcatenateCodepoints(int* codepoints1, int count1, int* codepoints2, int count2, int& count);

    /**
     * @brief 从字符串提取Unicode码点
     * @param text 输入字符串
     * @return 码点向量
     */
    std::vector<int> ExtractCodepointsFromText(const std::string& text);
}