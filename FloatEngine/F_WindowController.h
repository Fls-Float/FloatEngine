#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <functional>

// 引入 Raylib/Raymath (用于 Vector2 和 Rectangle)
#include "raylib.h"
#include "raymath.h"

#include "F_NetWork.h" 

// ----------------------------------------------------------------------------
// 1. 跨窗口通信数据结构
// ----------------------------------------------------------------------------

/**
 * @struct WindowTarget
 * @brief 描述一个远程或本地窗口的状态
 */
struct WindowTarget {
    std::string id;             ///< 窗口唯一 ID (通常是 IP:Port)
    int x;                      ///< 窗口在桌面上的全局 X 坐标
    int y;                      ///< 窗口在桌面上的全局 Y 坐标
    int width;                  ///< 窗口宽度
    int height;                 ///< 窗口高度
    bool is_mine = false;       ///< 是否是本地窗口
    bool is_active = true;      ///< 窗口是否存活
};

/**
 * @enum WindowProtocolID
 * @brief 窗口通信协议的 ID
 */
enum class WindowProtocolID : uint8_t {
    UPDATE_STATE = 1,           ///< 实时更新位置/大小/状态
    INTERACTION_REPORT = 2,     ///< 报告本地对象接触了远程窗口边界
    OBJECT_INCOMING = 3,        ///< 通知目标窗口生成一个来自外部的对象/效果
    DISCOVERY_REQUEST = 4,      ///< 请求发现其他窗口
};

// ----------------------------------------------------------------------------
// 2. 策略模式：协议处理接口
// ----------------------------------------------------------------------------

class F_WindowController; // 前置声明

/**
 * @class IProtocolHandler
 * @brief 协议处理的抽象基类 (策略模式)
 */
class IProtocolHandler {
public:
    virtual ~IProtocolHandler() = default;
    virtual void Handle(F_WindowController* controller,
        const std::string& senderID,
        const std::vector<unsigned char>& data) = 0;
};

// ----------------------------------------------------------------------------
// 3. F_WindowController 类定义
// ----------------------------------------------------------------------------

class F_WindowController {
public:
    F_WindowController();
    ~F_WindowController();

    F_WindowController(const F_WindowController&) = delete;
    F_WindowController& operator=(const F_WindowController&) = delete;

    /**
     * @brief 启动窗口控制器和本地网络服务
     * @param localPort 本地监听端口
     */
    void Start(int localPort);

    /**
     * @brief 核心更新函数 (每帧调用)
     */
    void Update();

    /**
     * @brief 尝试连接到另一个已知的窗口服务 (作为客户端)
     * @param ip 目标IP
     * @param port 目标端口
     */
    void ConnectToTarget(const std::string& ip, int port);

    /**
     * @brief 检查本地对象是否与任何远程窗口边界发生交互
     * @param localObjectRect 本地对象在窗口坐标系中的矩形
     * @param interactionData 附带的交互数据 (如伤害值、能量值等，用于报告)
     * @return true 如果与任何远程窗口边界接触
     */
    bool CheckBoundaryInteraction(rlRectangle localObjectRect, const std::vector<unsigned char>& interactionData);

    /**
     * @brief 获取所有窗口目标 (包括本地和远程)
     */
    const std::map<std::string, WindowTarget>& GetTargets() const { return m_targets; }

    /**
     * @brief 内部函数：更新远程窗口状态 (供协议处理器调用)
     */
    void UpdateRemoteState(const std::string& id, int x, int y, int w, int h, bool active);

private:
    std::unique_ptr<F_NetWork> m_net;
    std::string m_localID; // 本地窗口唯一标识 (IP:Port)

    // 窗口状态管理
    std::map<std::string, WindowTarget> m_targets;

    // 协议策略管理
    std::map<WindowProtocolID, std::unique_ptr<IProtocolHandler>> m_protocolMap;

    // 内部函数
    void SetupProtocolHandlers();
    void SendLocalStateUpdate();
    void ProcessNetworkEvents();
    std::string GetLocalIPAddress(); // 辅助函数：获取本地IP
};