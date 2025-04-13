/**
 * @file Object.h
 * @brief 游戏对象类
 * @defgroup Object 对象模块
 *
 * 提供游戏对象的创建、管理和事件处理功能：
 * - 对象属性管理
 * - 事件处理（用户事件、进入事件、更新事件、渲染事件等）
 * - 计时器功能
 */

#pragma once
#include "FloatApi.h"
#include <string>
#include "Enum.h"
#include <array>
#include <functional>

#define FOBJECT_BODY() float x, y; float image_index; Sprite sprite_index; int depth; Size m_block; float m_angle; Vector2 m_origin;float image_xscale;float image_yscale;float image_alpha;Color image_color;

 /**
  * @class Object
  * @ingroup Object
  *
  * @brief 提供游戏对象的创建、管理和事件处理功能
  *
  * @note 使用示例：
  * @code
  * Object obj("Player", 1);
  * obj.onUpdate();
  * obj.onRender();
  * @endcode
  */
class Object {
    int _m_obj_pro; ///< 对象属性
    std::string _m_obj_name; ///< 对象名称
    int __object_alarm_clock_timer_; ///< 计时器

protected:
    static const int ALARM_EVENT_COUNT = 11; ///< 计时器事件数量

    /**
     * @brief 重置计时器
     */
    void reset_alarm_clock();

    std::array<void (Object::*)(), ALARM_EVENT_COUNT> alarmEvents; ///< 计时器事件数组

    /**
     * @brief 显示对象计时器
     * @param font 字体
     * @param x x坐标
     * @param y y坐标
     * @param col 颜色
     */
    void show_object_alarm_clock_timer(Font font, float x, float y, Color col);

    int alarm_time_node[ALARM_EVENT_COUNT]; ///< 计时器节点数组

    /**
     * @brief 改变对象名称
     * @param name 新名称
     */
    void change_name(const char* name) {
        _m_obj_name = name;
    }

    /**
     * @brief 改变对象属性
     * @param pro 新属性
     */
    void change_pro(int pro) {
        _m_obj_pro = pro;
    }

    /**
     * @brief 绘制对象块
     * @param col 颜色
     */
    void Draw_Block(Color col);

    /**
     * @brief 判断对象是否相遇
     * @param _x x坐标
     * @param _y y坐标
     * @param other 另一个对象
     * @return 如果相遇返回true，否则返回false
     */
    bool Is_Meeting(float _x, float _y, const Object* other);

    /**
	* @brief 绘制自身
    */
    void Draw_Self();
public:
    int _ins_id; ///< 实例ID
    FOBJECT_BODY()

    /**
        * @brief 构造函数
    */
    Object();

    /**
     * @brief 构造函数
     * @param name 对象名称
     * @param ins_id 实例ID
     * @param pro 对象属性
     */
    Object(const char* name, int ins_id, int pro = OBJPRO.Normal);
  
    /**
      * @brief 移动构造函数
      * @param other 要移动的对象
     */
    Object(Object&& other) noexcept;

    /**
     * @brief 拷贝构造函数
     * @param other 要拷贝的对象
     */
    Object(const Object& other) ;

    /**
     * @brief 拷贝赋值运算符
     * @param other 要拷贝的对象
     * @return 拷贝后的对象引用
     */
    Object& operator=(const Object& other);

    /**
     * @brief 析构函数
     */
    ~Object();

    /**
     * @brief 设置对象名称
     * @param name 对象名称
     */
    inline void setObjName(const std::string& name) { _m_obj_name = name; }

    /**
     * @brief 设置对象属性
     * @param pro 对象属性
     */
    inline void setObjPro(const int& pro) { _m_obj_pro = pro; }

    /**
     * @brief 获取对象名称
     * @return 对象名称
     */
    inline const char* getObjName() const { return _m_obj_name.c_str(); }

    /**
     * @brief 获取对象属性
     * @return 对象属性
     */
    inline const int& getObjPro() const { return _m_obj_pro; }

    /**
     * @brief 用户事件处理
     * @param index 事件索引
     */
    virtual void onEvent_User(int index);

    /**
     * @brief 进入事件处理
     */
    virtual void onEnter();

    /**
     * @brief 更新事件处理
     */
    virtual void onUpdate();

    /**
     * @brief 渲染前事件处理
     */
    virtual void onRenderBefore();

    /**
     * @brief 渲染事件处理
     */
    virtual void onRender();

    /**
     * @brief 渲染后事件处理
     */
    virtual void onRenderNext();

    /**
     * @brief 摄像机开始事件处理
     */
    virtual void onBeginCamera();

    /**
     * @brief 摄像机结束事件处理
     */
    virtual void onEndCamera();

    /**
     * @brief 获取计时器事件数量
     * @return 计时器事件数量
     */
    int getAlarmCount() const {
        return ALARM_EVENT_COUNT;
    }

    /**
     * @brief 计时器事件处理
     */
    void onAlarmEvent();

    /**
     * @brief 计时器事件0处理
     */
    virtual void onAlarmEvent0();

    /**
     * @brief 计时器事件1处理
     */
    virtual void onAlarmEvent1();

    /**
     * @brief 计时器事件2处理
     */
    virtual void onAlarmEvent2();

    /**
     * @brief 计时器事件3处理
     */
    virtual void onAlarmEvent3();

    /**
     * @brief 计时器事件4处理
     */
    virtual void onAlarmEvent4();

    /**
     * @brief 计时器事件5处理
     */
    virtual void onAlarmEvent5();

    /**
     * @brief 计时器事件6处理
     */
    virtual void onAlarmEvent6();

    /**
     * @brief 计时器事件7处理
     */
    virtual void onAlarmEvent7();

    /**
     * @brief 计时器事件8处理
     */
    virtual void onAlarmEvent8();

    /**
     * @brief 计时器事件9处理
     */
    virtual void onAlarmEvent9();

    /**
     * @brief 计时器事件10处理
     */
    virtual void onAlarmEvent10();

    /**
     * @brief 销毁事件处理
     */
    virtual void onDestroy();
};


