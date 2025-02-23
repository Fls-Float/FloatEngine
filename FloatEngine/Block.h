#pragma once
#include "Object.h"

/**
 * @file Block.h
 * @brief 方块类
 * @defgroup Block 方块模块
 *
 * 提供方块的基本属性和行为：
 * - 方块的事件处理
 * - 注:暂时还未完成
 */

 /**
  * @class Block
  * @ingroup Block
  *
  * @brief 提供方块的基本属性和行为
  *
  * @note 使用示例：
  * @code
  * Block block;
  * block.onEvent_User(0);
  * @endcode
  */
class Block : public Object
{
public:
    /**
     * @brief 构造函数
     */
    Block();

    /**
     * @brief 析构函数
     */
    ~Block();

    /**
     * @brief 用户事件处理
     * @param index 事件索引
     */
    virtual void onEvent_User(int index) override;
};
