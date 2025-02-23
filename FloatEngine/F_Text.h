#pragma once
#include <iostream>
#include <raylib.h>
/**
 * @file F_Text.h
 * @brief �ı�������
 * @defgroup Text �ı�ģ��
 *
 * �ṩ�������ı��������ܣ�
 * - �ı���ֵ
 * - �ı��Ƚ�
 * - �ı�ת��
 * - �ı�ƴ��
 * - ��ȡ�ı�����
 */

 /**
  * @class F_Text
  * @ingroup Text
  *
  * @brief �ṩ�������ı���������
  *
  * @note ʹ��ʾ����
  * @code
  * F_Text text1("Hello");
  * F_Text text2("World");
  * text1 += text2;
  * std::cout << text1.cstr(); // ��� "HelloWorld"
  * @endcode
  */
class F_Text
{
    char* m_text;
public:
    /**
     * @brief ��ֵ����������
     * @param text Ҫ��ֵ���ı�
     * @return ���ظ�ֵ����ı�����
     */
    F_Text operator=(const char* text);

    /**
     * @brief ��ֵ����������
     * @param text Ҫ��ֵ���ı�����
     * @return ���ظ�ֵ����ı�����
     */
    F_Text operator=(const F_Text& text);

    /**
     * @brief �Ƚϲ���������
     * @param text Ҫ�Ƚϵ��ı�
     * @return �����ȷ���true�����򷵻�false
     */
    bool operator==(const char* text);

    /**
     * @brief �Ƚϲ���������
     * @param text Ҫ�Ƚϵ��ı�����
     * @return �����ȷ���true�����򷵻�false
     */
    bool operator==(const F_Text& text);

    /**
     * @brief ����ת������������
     * @return �����ı���C�ַ���
     */
    operator const char* ();

    /**
     * @brief ��ȡ�ı���C�ַ���
     * @return �����ı���C�ַ���
     */
    const char* cstr();

    /**
     * @brief ���ı�ת��Ϊstd::string
     * @return �����ı���std::string��ʾ
     */
    std::string tostr();

    /**
     * @brief ƴ�Ӳ���������
     * @param text Ҫƴ�ӵ��ı�����
     * @return ����ƴ�Ӻ���ı�����
     */
    F_Text operator+(const F_Text& text);

    /**
     * @brief ƴ�Ӳ���������
     * @param text Ҫƴ�ӵ��ı�
     * @return ����ƴ�Ӻ���ı�����
     */
    F_Text operator+(const char* text);

    /**
     * @brief ƴ�Ӳ���ֵ����������
     * @param text Ҫƴ�ӵ��ı�
     */
    void operator+=(const char* text);

    /**
     * @brief ƴ�Ӳ���ֵ����������
     * @param text Ҫƴ�ӵ��ı�����
     */
    void operator+=(const F_Text& text);

    /**
     * @brief ��ȡ�ı�����
     * @return �����ı��ĳ���
     */
    int Length();

    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Text() {
        m_text = new char[1];
    }

    /**
     * @brief ���캯��
     * @param text Ҫ��ʼ�����ı�
     */
    F_Text(const char* text);

    /**
     * @brief ��������
     */
    ~F_Text() {
        delete m_text;
    }
};

