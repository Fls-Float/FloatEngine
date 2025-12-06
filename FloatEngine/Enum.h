/**
 * @file Enum.h
 * @brief 枚举和常量定义
 * @defgroup Enum 枚举模块
 *
 * 提供方向、碰撞状态、对象属性和音频模式的枚举和常量定义：
 * - 方向常量
 * - 碰撞状态枚举
 * - 对象属性常量
 * - 音频模式枚举
 */
#pragma once

/**
    * @enum Collision
    * @ingroup Enum
    *
    * @brief 碰撞状态枚举
    */
enum Collision {
    Collision_None = 0, ///< 无碰撞
    Collision_With = 1, ///< 碰撞
    Collision_WithBlock = 2, ///< 与方块碰撞
    Collision_WithActor = 3, ///< 与角色碰撞
};

/**
    * @struct DIR
    * @ingroup Enum
    *
    * @brief 方向常量
    */
enum F_DIR {
    LEFT = 270, ///< 左
    RIGHT = 90, ///< 右
    UP = 0, ///< 上
    DOWN = 180, ///< 下
};

