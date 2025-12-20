# FloatEngine 

**一个高性能、跨平台的 C++ 游戏引擎，由 [fls-float](https://github.com/fls-float) 于 2023 年立项开发。**

<p align="center">
    <img src="https://img.shields.io/badge/Language-C++17-blue.svg?style=for-the-badge" />
    <img src="https://img.shields.io/badge/Platform-Win%20.svg?style=for-the-badge" />
    <img src="https://img.shields.io/badge/Build%20System-CMake%20|%20XMake-orange.svg?style=for-the-badge" />
    <img src="https://img.shields.io/badge/Package%20Manager-Vcpkg%20|%20NuGet-red.svg?style=for-the-badge" />
</p>

---

##  核心特性 (Features)

FloatEngine 的目标是提供一个高性能、易于使用的游戏开发框架，专注于底层性能和模块化设计。

| 模块 | 功能描述 | 关键技术 | 优势 |
| :--- | :--- | :--- | :--- |
| **图形/核心** | 2D/3D 渲染、窗口管理 | [Raylib](https://github.com/raysan5/raylib) (定制修复版) | 极简 API，原生跨平台渲染，Win32 冲突已解决。 |
| **网络/多人** | 可靠 UDP (RUDP) 实时通信 | [SLikeNet (RakNet)](https://github.com/SLikeNet/SLikeNet) | 内置对象复制、RPC，高性能低延迟。 |
| **UI/Web** | 异步 HTTP/HTTPS 交互 | [Libcurl](https://github.com/curl/curl) | 支持 Web API 交互 (排行榜、登录)、DeepSeek LLM 等服务。 |
| **数据/脚本** | JSON 数据处理、脚本化游戏逻辑 | [F_Json](https://github.com/fls-float/FloatEngine) (基于 nlohmann/json) | C++ 驱动的 Lua/Sol2 脚本系统。 |
| **架构** | 实体组件系统 (ECS) 基础、对象池 | `Room`/`Object` 模块 | 严格的智能指针内存管理 (RAII)，优化游戏性能。 |

## 构建系统 (Build System)

FloatEngine 提供了两种主流的构建方案：

### 1. CMake + Vcpkg 

这是最主流的 C++ 解决方案，用于实现跨平台和静态链接。

```bash
# 1. 克隆项目
git clone https://github.com/fls-float/FloatEngine.git
cd FloatEngine

# 2. 配置 
cmake -B build -S .   -DCMAKE_TOOLCHAIN_FILE="<YOUR_VCPKG_PATH>/scripts/buildsystems/vcpkg.cmake"  -DVCPKG_TARGET_TRIPLET=x64-windows-static

# 3. 编译
cmake --build build --config Release

```

### 2. XMake
XMake 是一个高效的现代构建工具，内置了依赖管理，配置更加简洁。

```bash
# 1. 克隆项目
git clone https://github.com/fls-float/FloatEngine-Xmake.git
cd FloatEngine

# 2. 构建
xmake build

```


## 配置 (Setup)

### 1. Visual Studio + NuGet

这是最方便的配置方法，右键解决方案管理器，点击管理NuGet包，搜索FloatEngine

### 2. 下载发布版手动配置

较为复杂，需要下载github的发布版再使用IDE分别配置include和lib并手动链接


