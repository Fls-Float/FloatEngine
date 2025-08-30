/**
 * @file Sprite.h
 * @brief 游戏精灵类
 * @defgroup Sprite 精灵模块
 */
#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "raylib.hpp"
#include "F_Resource.h"
class Sprite {
private:
    std::vector<Texture> frames; ///< 存储帧的向量
    size_t currentFrame = 0; ///< 当前帧的索引
    bool isOwner = true; ///< 是否拥有帧资源的所有权

public:
    /**
     * @brief 默认构造函数
     */
    Sprite();

    /**
     * @brief 使用单个纹理构造Sprite
     * @param texture 纹理
     * @param takeOwnership 是否拥有纹理资源的所有权
     */
    explicit Sprite(Texture2D texture, bool takeOwnership = true);

    /**
     * @brief 使用文件路径列表构造Sprite
     * @param filePaths 文件路径列表
     */
    explicit Sprite(const std::vector<const char*>& filePaths);

    /**
     * @brief 析构函数
     */
    ~Sprite();

    /**
     * @brief 拷贝构造函数
     */
    Sprite(const Sprite&other) ;

    /**
     * @brief 拷贝赋值运算符
     */
    Sprite& operator=(const Sprite& other);

    /**
     * @brief 移动构造函数
     * @param other 要移动的对象
     */
    Sprite(Sprite&& other) noexcept;

    /**
     * @brief 移动赋值运算符
     * @param other 要移动的对象
     * @return 移动后的对象引用
     */
    Sprite& operator=(Sprite&& other) noexcept;

    /**
     * @brief 添加帧
     * @param texture 要添加的纹理
     */
    void AddFrame(const Texture2D&& texture);

    /**
     * @brief 从文件加载帧
     * @param filePath 文件路径
     * @return 加载成功返回true，否则返回false
     */
    bool LoadFrame(const char* filePath);
    /**
    * @brief 从F_Resource加载帧
    * @param resource 资源类
    * @param name 资源名称
    * @return 加载成功返回true，否则返回false
    */
    bool LoadFrameFromResource(const F_Resource* resource, const char* name);
    /**
     * @brief 设置当前帧
     * @param index 帧索引
     */
    void SetFrame(size_t index);
    /**
	* @brief 更新帧索引（+1）超出范围则重置为0
    */
    void UpdateFrameIndex();
    /**
    * @brief 修改帧纹理
    * @param index 帧索引
    */
    void SetFrameTexture(size_t index,const Texture& tex);
    /**
     * @brief 获取当前帧
     * @return 当前帧的纹理
     */
    Texture2D GetCurrentFrame() const;

    /**
     * @brief 获取当前帧
     * @return 当前帧的纹理
     */
    Texture2D GetFrameIndex(int index) const;
    /**
    * @brief 获取所有帧
    * @return 所有帧的纹理
    */
    Texture2D* GetAllFrame() const;
    /**
     * @brief 获取帧的数量
     * @return 帧的数量
     */
    size_t FrameCount() const;
    
};
