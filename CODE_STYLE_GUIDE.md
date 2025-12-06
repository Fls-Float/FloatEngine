# 浮点引擎代码规范指南

## 1. 命名规范

### 1.1 类命名
- **规则**：使用 `PascalCase`（帕斯卡命名法）
- **示例**：`Object`、`F_Resource`、`ObjectManager`
- **前缀说明**：
  - `F_` 前缀：表示 FloatEngine API 类
  - `I` 前缀：表示接口类（如 `IObjectCreator`）

### 1.2 方法/函数命名
- **规则**：使用 `camelCase`（驼峰命名法）
- **动词优先**：以动词开头表示动作
- **示例**：
  ```cpp
  void loadResources();      // 加载资源
  bool isTypeRegistered();   // 检查类型是否已注册
  void cleanup();            // 清理资源
  ```

### 1.3 成员变量命名
- **规则**：使用 `snake_case` 或 `m_` 前缀
- **私有成员**：使用 `_` 前缀或 `m_` 前缀
- **示例**：
  ```cpp
  private:
      std::string _zip_path;           // 推荐
      int object_id_;                  // 备选
      Texture2D* texture_array;        // 清晰的指针名称
  ```

### 1.4 常量命名
- **规则**：使用 `UPPER_SNAKE_CASE`
- **示例**：
  ```cpp
  static const int ALARM_EVENT_COUNT = 11;
  static const int MAX_OBJECT_INSTANCES = 10000;
  ```

### 1.5 枚举命名
- **规则**：使用 `PascalCase`
- **示例**：
  ```cpp
  enum class ResourceType {
      ResTexture,    // 推荐：使用 PascalCase
      ResSound,
      ResMusic
  };
  ```

## 2. 代码结构规范

### 2.1 包含文件顺序
```cpp
#pragma once

// 1. 系统标准库
#include <string>
#include <vector>
#include <memory>
#include <array>

// 2. 第三方库
#include "raylib.hpp"

// 3. 项目内部库
#include "FloatApi.h"
#include "Enum.h"
```

### 2.2 类声明顺序
```cpp
class MyClass {
public:
    // 1. 类型定义和嵌套类
    using ValueType = int;
    
    // 2. 构造函数和析构函数
    MyClass();
    ~MyClass();
    
    // 3. 删除的函数
    MyClass(const MyClass&) = delete;
    MyClass& operator=(const MyClass&) = delete;
    
    // 4. 移动语义
    MyClass(MyClass&&) noexcept;
    MyClass& operator=(MyClass&&) noexcept;
    
    // 5. 公共方法
    void publicMethod();
    
    // 6. 公共成员变量（尽量避免）
    
protected:
    // 保护成员
    
private:
    // 私有方法
    void privateMethod();
    
    // 私有成员变量
    std::string _data;
};
```

## 3. 内存管理规范

### 3.1 使用智能指针
```cpp
// ? 不推荐：手动内存管理
Object* obj = new Object();
delete obj;

// ? 推荐：使用 std::unique_ptr（单一所有权）
std::unique_ptr<Object> obj = std::make_unique<Object>();

// ? 推荐：使用 std::shared_ptr（共享所有权）
std::shared_ptr<Object> obj = std::make_shared<Object>();

// ? 推荐：容器使用智能指针
std::vector<std::unique_ptr<Object>> objects;
```

### 3.2 RAII 原则
```cpp
class ResourceManager {
public:
    ResourceManager() {
        // 获取资源
        resource = allocateResource();
    }
    
    ~ResourceManager() {
        // 自动释放资源
        if (resource) {
            freeResource(resource);
            resource = nullptr;
        }
    }
    
private:
    Resource* resource = nullptr;
};
```

### 3.3 避免内存泄漏
```cpp
// ? 不推荐：容器存储原始指针
std::vector<Object*> objects;
objects.push_back(new Object());  // 容易泄漏

// ? 推荐：容器存储智能指针
std::vector<std::unique_ptr<Object>> objects;
objects.push_back(std::make_unique<Object>());

// ? 推荐：确保所有权清晰
class Container {
    std::vector<std::shared_ptr<Object>> owned_objects;
public:
    void addObject(std::shared_ptr<Object> obj) {
        owned_objects.push_back(obj);  // 容器拥有所有权
    }
};
```

## 4. C++17 最佳实践

### 4.1 结构化绑定
```cpp
// ? 使用结构化绑定简化代码
auto [name, type, path] = getResourceInfo();

// ? 在循环中使用
for (const auto& [key, value] : resources_map) {
    processResource(key, value);
}
```

### 4.2 `std::optional`
```cpp
#include <optional>

// ? 推荐：使用 optional 表示可能不存在的值
std::optional<Texture2D> loadTexture(const std::string& path);

if (auto texture = loadTexture("path")) {
    renderTexture(*texture);
}
```

### 4.3 `std::variant`
```cpp
#include <variant>

// ? 推荐：类型安全的联合体
std::variant<Texture2D, Sound, Font> resource;

if (std::holds_alternative<Texture2D>(resource)) {
    auto& texture = std::get<Texture2D>(resource);
}
```

## 5. 代码格式规范

### 5.1 缩进和间距
```cpp
// ? 推荐：4 空格缩进
void myFunction() {
    if (condition) {
        doSomething();
    }
}

// ? 推荐：运算符周围有空格
int result = a + b * c;
```

### 5.2 函数长度
- 单个函数不应超过 50-100 行
- 将复杂逻辑分解为小函数
- 提高代码可读性和可维护性

### 5.3 文件长度
- 单个文件不应超过 500-1000 行
- 将相关功能分组到不同的文件中

## 6. 错误处理规范

### 6.1 异常处理
```cpp
// ? 推荐：使用异常处理
try {
    auto resource = loadCriticalResource();
} catch (const std::runtime_error& e) {
    DEBUG_LOG(LOG_ERROR, TextFormat("Failed to load: %s", e.what()), false);
}

// ? 推荐：使用 noexcept
void cleanup() noexcept {
    // 清理代码，保证不抛出异常
}
```

### 6.2 返回值检查
```cpp
// ? 推荐：检查返回值
if (!loadResource(path)) {
    DEBUG_LOG(LOG_ERROR, "Failed to load resource", false);
    return false;
}
```

## 7. 文档注释规范

### 7.1 Doxygen 注释
```cpp
/**
 * @brief 加载资源文件
 * @param path 资源文件路径
 * @param type 资源类型
 * @return 加载成功返回 true，否则返回 false
 * @throw std::runtime_error 当资源加载失败时抛出
 * @note 此函数会自动缓存已加载的资源
 * @see unloadResource
 */
bool loadResource(const std::string& path, ResourceType type);
```

### 7.2 代码注释
```cpp
// ? 推荐：清晰的注释说明"为什么"而不是"是什么"
// 由于纹理需要在渲染前加载完成，这里使用同步加载
Texture2D texture = loadTextureSync(path);

// ? 不推荐：明显的重复
int count = 0;  // 设置 count 为 0
```

## 8. 完整示例

### 8.1 改进的类设计
```cpp
/**
 * @class ResourceCache
 * @brief 资源缓存管理器
 */
class ResourceCache {
public:
    ResourceCache() = default;
    ~ResourceCache() = default;
    
    // 删除拷贝语义
    ResourceCache(const ResourceCache&) = delete;
    ResourceCache& operator=(const ResourceCache&) = delete;
    
    // 移动语义
    ResourceCache(ResourceCache&&) noexcept = default;
    ResourceCache& operator=(ResourceCache&&) noexcept = default;
    
    /**
     * @brief 添加资源到缓存
     * @param key 资源键
     * @param resource 资源对象
     */
    void addResource(const std::string& key, 
                     std::shared_ptr<Object> resource) {
        if (!resource) {
            throw std::invalid_argument("Resource cannot be null");
        }
        _resources[key] = resource;
    }
    
    /**
     * @brief 获取缓存的资源
     * @param key 资源键
     * @return 缓存的资源指针，如果不存在返回 nullptr
     */
    std::shared_ptr<Object> getResource(const std::string& key) const {
        auto it = _resources.find(key);
        return (it != _resources.end()) ? it->second : nullptr;
    }
    
    /**
     * @brief 清空所有缓存
     */
    void clear() noexcept {
        _resources.clear();
    }
    
private:
    std::unordered_map<std::string, std::shared_ptr<Object>> _resources;
};
```

## 9. 检查清单

- [ ] 所有类都遵循 PascalCase 命名
- [ ] 所有方法都遵循 camelCase 命名
- [ ] 使用智能指针而不是原始指针
- [ ] RAII 原则得到遵循
- [ ] 所有公共接口都有 Doxygen 注释
- [ ] 异常处理得到正确实现
- [ ] 没有内存泄漏（通过内存分析工具验证）
- [ ] 代码格式一致
- [ ] 单元测试覆盖关键功能
