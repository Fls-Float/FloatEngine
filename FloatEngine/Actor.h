#pragma once
#include "Object.h"
#include "raymath.hpp"
/**
 * @file Actor.h
 * @brief 角色类
 * @defgroup Actor 角色模块
 *
 * 提供角色的基本属性和行为：
 * - 角色的移动、跳跃等属性
 * - 角色的重力设置
 * - 注:暂时还未完成
 */

 /**
  * @class Actor
  * @ingroup Actor
  *
  * @brief 提供角色的基本属性和行为
  *
  * @note 使用示例：
  * @code
  * Actor actor;
  * actor.active = true;
  * actor.gravity = FVec2(0, 9.8);
  * @endcode
  */
class Actor : public Object
{
protected:
public:
    /**
     * @brief 构造函数
     */
    Actor();

    /**
     * @brief 析构函数
     */
    ~Actor();

    bool active; ///< 角色是否激活
    bool moveable; ///< 角色是否可移动
    bool jumpable; ///< 角色是否可跳跃
    bool isJump; ///< 角色是否在跳跃中
    float moveVelocity; ///< 角色速度
    float moveDirection; ///< 角色方向 

    FVec2 gravity; ///< 角色的重力
    bool enableGravity; ///< 是否启用重力
    /**
      * @brief 处理物理的函数
      * @param deltaTime 每帧的时间
      */
    void UpdatePhysics(float deltaTime);
    
};
