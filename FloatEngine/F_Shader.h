#pragma once
#include <raylib.h>
/**
 * @file F_Shader.h
 * @brief ��ɫ��������
 * @defgroup Shader ��ɫ��ģ��
 *
 * �ṩ��ɫ���ļ��ء����ú͹����ܣ�
 * - ���ļ�����������ɫ��
 * - ������ɫ������
 * - ������ɫ����������
 */

 /**
  * @class F_Shader
  * @ingroup Shader
  *
  * @brief �ṩ��ɫ���ļ��ء����ú͹�����
  *
  * @note ʹ��ʾ����
  * @code
  * F_Shader shader;
  * shader.LoadFromFile("vertex.vs", "fragment.fs");
  * shader.Begin();
  * // ������ɫ������
  * shader.End();
  * @endcode
  */
class F_Shader
{
    Shader m_shader;
public:
    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Shader();

    /**
     * @brief ���캯��
     * @param shader ��ɫ������
     */
    F_Shader(const Shader& shader);

    /**
     * @brief ���ļ�������ɫ��
     * @param vs ������ɫ���ļ�·��
     * @param fs Ƭ����ɫ���ļ�·��
     */
    void LoadFromFile(const char* vs, const char* fs);

    /**
     * @brief �Ӵ��������ɫ��
     * @param vs ������ɫ������
     * @param fs Ƭ����ɫ������
     */
    void LoadFromCode(const char* vs, const char* fs);

    /**
     * @brief ������ɫ������
     * @param name ��������
     * @param value ����ֵ
     * @param type ��������
     */
    void SetValue(const char* name, const void* value, int type);

    /**
     * @brief �����������
     * @param name ��������
     * @param texture �������
     */
    void SetValueTexture(const char* name, Texture texture);

    /**
     * @brief ���þ������
     * @param name ��������
     * @param mat �������
     */
    void SetrValueMatrix(const char* name, Matrix mat);

    /**
     * @brief ��ȡ����λ��
     * @param name ��������
     * @return ����λ��
     */
    int GetLoc(const char* name);

    /**
     * @brief ж����ɫ��
     */
    void Unload();

    /**
     * @brief ��ʼʹ����ɫ��
     */
    void Begin();

    /**
     * @brief ����ʹ����ɫ��
     */
    void End();
};
