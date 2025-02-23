#pragma once
#include <raylib.h>
/**
 * @file F_Shader.h
 * @brief 着色器管理类
 * @defgroup Shader 着色器模块
 *
 * 提供着色器的加载、设置和管理功能：
 * - 从文件或代码加载着色器
 * - 设置着色器参数
 * - 管理着色器生命周期
 */

 /**
  * @class F_Shader
  * @ingroup Shader
  *
  * @brief 提供着色器的加载、设置和管理功能
  *
  * @note 使用示例：
  * @code
  * F_Shader shader;
  * shader.LoadFromFile("vertex.vs", "fragment.fs");
  * shader.Begin();
  * // 设置着色器参数
  * shader.End();
  * @endcode
  */
class F_Shader
{
    Shader m_shader;
public:
    /**
     * @brief 默认构造函数
     */
    F_Shader();

    /**
     * @brief 构造函数
     * @param shader 着色器对象
     */
    F_Shader(const Shader& shader);

    /**
     * @brief 从文件加载着色器
     * @param vs 顶点着色器文件路径
     * @param fs 片段着色器文件路径
     */
    void LoadFromFile(const char* vs, const char* fs);

    /**
     * @brief 从代码加载着色器
     * @param vs 顶点着色器代码
     * @param fs 片段着色器代码
     */
    void LoadFromCode(const char* vs, const char* fs);

    /**
     * @brief 设置着色器参数
     * @param name 参数名称
     * @param value 参数值
     * @param type 参数类型
     */
    void SetValue(const char* name, const void* value, int type);

    /**
     * @brief 设置纹理参数
     * @param name 参数名称
     * @param texture 纹理对象
     */
    void SetValueTexture(const char* name, Texture texture);

    /**
     * @brief 设置矩阵参数
     * @param name 参数名称
     * @param mat 矩阵对象
     */
    void SetrValueMatrix(const char* name, Matrix mat);

    /**
     * @brief 获取参数位置
     * @param name 参数名称
     * @return 参数位置
     */
    int GetLoc(const char* name);

    /**
     * @brief 卸载着色器
     */
    void Unload();

    /**
     * @brief 开始使用着色器
     */
    void Begin();

    /**
     * @brief 结束使用着色器
     */
    void End();
};
