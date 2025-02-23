#pragma once
#include <iostream>
#include <raylib.h>
/**
 * @file F_Text.h
 * @brief 文本处理类
 * @defgroup Text 文本模块
 *
 * 提供基本的文本操作功能：
 * - 文本赋值
 * - 文本比较
 * - 文本转换
 * - 文本拼接
 * - 获取文本长度
 */

 /**
  * @class F_Text
  * @ingroup Text
  *
  * @brief 提供基本的文本操作功能
  *
  * @note 使用示例：
  * @code
  * F_Text text1("Hello");
  * F_Text text2("World");
  * text1 += text2;
  * std::cout << text1.cstr(); // 输出 "HelloWorld"
  * @endcode
  */
class F_Text
{
    char* m_text;
public:
    /**
     * @brief 赋值操作符重载
     * @param text 要赋值的文本
     * @return 返回赋值后的文本对象
     */
    F_Text operator=(const char* text);

    /**
     * @brief 赋值操作符重载
     * @param text 要赋值的文本对象
     * @return 返回赋值后的文本对象
     */
    F_Text operator=(const F_Text& text);

    /**
     * @brief 比较操作符重载
     * @param text 要比较的文本
     * @return 如果相等返回true，否则返回false
     */
    bool operator==(const char* text);

    /**
     * @brief 比较操作符重载
     * @param text 要比较的文本对象
     * @return 如果相等返回true，否则返回false
     */
    bool operator==(const F_Text& text);

    /**
     * @brief 类型转换操作符重载
     * @return 返回文本的C字符串
     */
    operator const char* ();

    /**
     * @brief 获取文本的C字符串
     * @return 返回文本的C字符串
     */
    const char* cstr();

    /**
     * @brief 将文本转换为std::string
     * @return 返回文本的std::string表示
     */
    std::string tostr();

    /**
     * @brief 拼接操作符重载
     * @param text 要拼接的文本对象
     * @return 返回拼接后的文本对象
     */
    F_Text operator+(const F_Text& text);

    /**
     * @brief 拼接操作符重载
     * @param text 要拼接的文本
     * @return 返回拼接后的文本对象
     */
    F_Text operator+(const char* text);

    /**
     * @brief 拼接并赋值操作符重载
     * @param text 要拼接的文本
     */
    void operator+=(const char* text);

    /**
     * @brief 拼接并赋值操作符重载
     * @param text 要拼接的文本对象
     */
    void operator+=(const F_Text& text);

    /**
     * @brief 获取文本长度
     * @return 返回文本的长度
     */
    int Length();

    /**
     * @brief 默认构造函数
     */
    F_Text() {
        m_text = new char[1];
    }

    /**
     * @brief 构造函数
     * @param text 要初始化的文本
     */
    F_Text(const char* text);

    /**
     * @brief 析构函数
     */
    ~F_Text() {
        delete m_text;
    }
};

