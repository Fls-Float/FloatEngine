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
struct {
    static constexpr float LEFT = 270.0f; ///< 左
    static constexpr float RIGHT = 90.0f; ///< 右
    static constexpr float UP = 0.0f; ///< 上
    static constexpr float DOWN = 180.0f; ///< 下
} DIR;

/**
 * @struct OBJPRO
 * @ingroup Enum
 *
 * @brief 对象属性常量
 */
struct {
    static constexpr int Normal = 0; ///< 普通对象
    static constexpr int Actor = 1; ///< 角色对象
    static constexpr int Block = 2; ///< 方块对象
} OBJPRO;

/**
 * @enum AudioMode
 * @ingroup Enum
 *
 * @brief 音频模式枚举
 */
enum AudioMode {
    Audio_Unknown = 0, ///< 未知音频模式
    Audio_Sound = 1, ///< 声音模式
    Audio_Music = 2, ///< 音乐模式
};
