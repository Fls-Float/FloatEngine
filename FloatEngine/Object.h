/**
 * @file Object.h
 * @brief 游戏对象基类
 * @defgroup Object 对象模块
 *
 * 提供游戏对象的创建、管理和事件处理功能：
 * - 对象属性管理
 * - 事件处理（用户事件、进入事件、更新事件、渲染事件等）
 * - 计时器功能
 * - 生命周期管理
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
    bool object_mode_no_destroy_sprite;

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
    virtual ~Object();

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
/**
 * @brief 对象创建器接口
 */
class IObjectCreator {
public:
    virtual ~IObjectCreator() = default;

    /**
     * @brief 创建对象实例
     * @param args 创建参数
     */
    virtual std::shared_ptr<Object> create(sol::variadic_args args) = 0;

    /**
     * @brief 获取对象类型信息
     */
    virtual const std::type_info& getTypeInfo() const = 0;

    /**
     * @brief 注册到 Lua
     */
    virtual void registerToLua(sol::state& lua, const std::string& name) = 0;
};

/**
 * @brief 模板对象创建器
 */
template<typename T>
class ObjectCreator : public IObjectCreator {
    static_assert(std::is_base_of_v<Object, T>, "T must inherit from Object");

public:
    std::shared_ptr<Object> create(sol::variadic_args args) override {
        return createImpl(args);
    }

    const std::type_info& getTypeInfo() const override {
        return typeid(T);
    }

    void registerToLua(sol::state& lua, const std::string& name) override {
        registerToLuaImpl(lua, name);
    }

private:
    /**
     * @brief 具体的创建实现，支持不同数量的参数
     */
    template<typename... Args>
    std::shared_ptr<Object> createWithArgs(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    std::shared_ptr<Object> createImpl(sol::variadic_args args) {
        const size_t argCount = args.size();

        // 根据参数数量分派到不同的创建函数
        switch (argCount) {
        case 0:
            return createWithArgs();
        case 1:
            if (args[0].is<const char*>()) {
                return createWithArgs(args[0].as<const char*>());
            }
            else if (args[0].is<std::string>()) {
                return createWithArgs(args[0].as<std::string>().c_str());
            }
            else if (args[0].is<int>()) {
                return createWithArgs("", args[0].as<int>());
            }
            break;
        case 2:
            if (args[0].is<const char*>() && args[1].is<int>()) {
                return createWithArgs(args[0].as<const char*>(), args[1].as<int>());
            }
            else if (args[0].is<std::string>() && args[1].is<int>()) {
                return createWithArgs(args[0].as<std::string>().c_str(), args[1].as<int>());
            }
            break;
        case 3:
            if (args[0].is<const char*>() && args[1].is<int>() && args[2].is<int>()) {
                return createWithArgs(args[0].as<const char*>(), args[1].as<int>(), args[2].as<int>());
            }
            else if (args[0].is<std::string>() && args[1].is<int>() && args[2].is<int>()) {
                return createWithArgs(args[0].as<std::string>().c_str(), args[1].as<int>(), args[2].as<int>());
            }
            break;
        default:
            throw std::runtime_error("Too many arguments for object creation");
        }

        throw std::runtime_error("Invalid argument types for object creation");
    }

    void registerToLuaImpl(sol::state& lua, const std::string& name) {
        // 注册 Object 类到 Lua，支持虚函数重载
        lua.new_usertype<Object>(
            name,
            sol::base_classes, sol::bases<Object>(),
            sol::call_constructor, sol::constructors<
            Object(),
            Object(const char*, int, int),
            Object(Object&&),
            Object(const Object&)
            >(),

            // 属性
            "_ins_id", &Object::_ins_id,
            "_m_obj_pro", &Object::_m_obj_pro,
            "_m_obj_name", &Object::_m_obj_name,

            // 公共方法
            "setObjName", &Object::setObjName,
            "setObjPro", &Object::setObjPro,
            "getObjName", &Object::getObjName,
            "getObjPro", &Object::getObjPro,
            "getAlarmCount", &Object::getAlarmCount,

            // 虚函数 - 使用 override 支持 Lua 重载
            "onEvent_User", (&Object::onEvent_User),
            "onEnter", (&Object::onEnter),
            "onUpdate", (&Object::onUpdate),
            "onRenderBefore", (&Object::onRenderBefore),
            "onRender", (&Object::onRender),
            "onRenderNext", (&Object::onRenderNext),
            "onBeginCamera", (&Object::onBeginCamera),
            "onEndCamera", (&Object::onEndCamera),
            "onDestroy", (&Object::onDestroy),

            // 计时器虚函数
            "onAlarmEvent0", (&Object::onAlarmEvent0),
            "onAlarmEvent1", (&Object::onAlarmEvent1),
            "onAlarmEvent2", (&Object::onAlarmEvent2),
            "onAlarmEvent3", (&Object::onAlarmEvent3),
            "onAlarmEvent4", (&Object::onAlarmEvent4),
            "onAlarmEvent5", (&Object::onAlarmEvent5),
            "onAlarmEvent6", (&Object::onAlarmEvent6),
            "onAlarmEvent7", (&Object::onAlarmEvent7),
            "onAlarmEvent8", (&Object::onAlarmEvent8),
            "onAlarmEvent9", (&Object::onAlarmEvent9),
            "onAlarmEvent10", (&Object::onAlarmEvent10),

            // 其他方法
            "onAlarmEvent", &Object::onAlarmEvent,
            "change_name", &Object::change_name,
            "change_pro", &Object::change_pro
        );

        // 如果注册的是派生类，也需要注册其特定方法
        if constexpr (!std::is_same_v<T, Object>) {
            registerDerivedClassToLua(lua, name);
        }
    }

    /**
     * @brief 为派生类注册额外的方法
     */
    template<typename U = T>
    typename std::enable_if<!std::is_same_v<U, Object>>::type
        registerDerivedClassToLua(sol::state& lua, const std::string& name) {
        // 为派生类创建新的 usertype，继承自 Object
        auto derived_usertype = lua.new_usertype<U>(
            name,
            sol::base_classes, sol::bases<Object>(),
            sol::call_constructor, sol::constructors<
            U(),
            U(const char*, int, int),
            U(U&&),
            U(const U&)
            >()
        );

        // 自动检测并注册派生类的公共方法
      //  registerDerivedMethods<U>(derived_usertype);

        // 自动检测并注册派生类的额外虚函数重载
      //  registerDerivedVirtualMethods<U>(derived_usertype);

        std::cout << "Registered derived class: " << name << " with additional methods" << std::endl;
    }
};

/**
 * @brief Lua 对象创建器（用于在 Lua 中定义的对象类型）
 */
class LuaObjectCreator : public IObjectCreator {
public:
    LuaObjectCreator(sol::function lua_constructor)
        : lua_constructor_(lua_constructor) {
    }

    std::shared_ptr<Object> create(sol::variadic_args args) override {
        sol::object result = lua_constructor_(args);
        if (result.is<std::shared_ptr<Object>>()) {
            return result.as<std::shared_ptr<Object>>();
        }
        throw std::runtime_error("Lua constructor did not return an Object");
    }

    const std::type_info& getTypeInfo() const override {
        return typeid(Object); // Lua 对象都视为 Object 类型
    }

    void registerToLua(sol::state& lua, const std::string& name) override {
        // Lua 类型已经在 Lua 中定义，不需要额外注册
    }

private:
    sol::function lua_constructor_;
};

/**
 * @brief 对象管理器类
 */
class ObjectManager {
private:
    std::unordered_map<std::string, std::unique_ptr<IObjectCreator>> object_creators_;
    std::unordered_map<std::string, std::string> type_to_name_;
    sol::state* lua_state_ = nullptr;

    ObjectManager() = default;

public:
    // 禁用拷贝和移动
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
    ObjectManager(ObjectManager&&) = delete;
    ObjectManager& operator=(ObjectManager&&) = delete;

    /**
     * @brief 获取单例实例
     */
    static ObjectManager& getInstance() {
        static ObjectManager instance;
        return instance;
    }

    /**
     * @brief 设置 Lua 状态
     */
    void setLuaState(sol::state& lua) {
        lua_state_ = &lua;
    }

    /**
     * @brief 注册 C++ 对象类型
     */
    template<typename T>
    void registerObjectType(const std::string& name) {
        static_assert(std::is_base_of_v<Object, T>, "T must inherit from Object");

        if (object_creators_.find(name) != object_creators_.end()) {
            throw std::runtime_error("Object type '" + name + "' is already registered");
        }

        object_creators_[name] = std::make_unique<ObjectCreator<T>>();
        type_to_name_[typeid(T).name()] = name;

        // 如果 Lua 状态已设置，自动注册到 Lua
        if (lua_state_) {
            object_creators_[name]->registerToLua(*lua_state_, name);
        }

        std::cout << "Registered object type: " << name << " (" << typeid(T).name() << ")" << std::endl;
    }

    /**
     * @brief 注册 Lua 对象类型
     */
    void registerLuaObjectType(const std::string& name, sol::function constructor);

    /**
     * @brief 创建对象实例
     */
    std::shared_ptr<Object> createInstance(const std::string& name, sol::variadic_args args);

    /**
     * @brief 检查对象类型是否已注册
     */
    bool isTypeRegistered(const std::string& name) const;

    /**
     * @brief 获取已注册的类型名称列表
     */
    std::vector<std::string> getRegisteredTypeNames() const;

    /**
     * @brief 根据类型信息获取类型名称
     */
    std::string getTypeName(const std::type_info& type) const;

    /**
     * @brief 获取对象创建器
     */
    IObjectCreator* getCreator(const std::string& name) const;

    /**
     * @brief 清空所有注册的类型
     */
    void clear();

    /**
     * @brief 注册全局函数到 Lua
     */
    void registerGlobalFunctions();
};