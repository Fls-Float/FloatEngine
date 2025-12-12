#include "F_WindowController.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <chrono>

#include "raylib.h"
#include "raymath.h"
// ----------------------------------------------------------------------------
// 1. 协议策略实现 (IProtocolHandler 派生类)
// ----------------------------------------------------------------------------

/**
 * @brief 从原始数据中提取基本类型 (使用 std::memcpy)
 */
template<typename T>
T Extract(const unsigned char*& ptr) {
    T value;
    std::memcpy(&value, ptr, sizeof(T));
    ptr += sizeof(T);
    return value;
}

/**
 * @class UpdateStateHandler
 * @brief 处理 WindowProtocolID::UPDATE_STATE 消息 (远程窗口位置更新)
 */
class UpdateStateHandler : public IProtocolHandler {
public:
    void Handle(F_WindowController* controller,
        const std::string& senderID,
        const std::vector<unsigned char>& data) override
    {
        if (data.size() < 4 * sizeof(int) + sizeof(bool)) return;

        const unsigned char* ptr = data.data();

        int x = Extract<int>(ptr);
        int y = Extract<int>(ptr);
        int w = Extract<int>(ptr);
        int h = Extract<int>(ptr);
        bool active = Extract<bool>(ptr);

        // 更新控制器中的状态
        controller->UpdateRemoteState(senderID, x, y, w, h, active);
    }
};

/**
 * @class InteractionReportHandler
 * @brief 处理 WindowProtocolID::INTERACTION_REPORT 消息 (本地窗口被击中/交互)
 */
class InteractionReportHandler : public IProtocolHandler {
public:
    void Handle(F_WindowController* controller,
        const std::string& senderID,
        const std::vector<unsigned char>& data) override
    {
        // 假设 interactionData 就是原始的字节数据
        // 在这里，你可以根据 data 的内容，调用游戏逻辑来处理交互，比如：
        // F_Game::ProcessIncomingInteraction(senderID, data);

        std::cout << "[GAME INTERACTION] Local window touched by " << senderID
            << ". Data size: " << data.size() << " bytes." << std::endl;

        // 游戏逻辑: 例如，如果第一个字节是伤害值
        if (!data.empty()) {
            int damage = data[0];
            // F_Game::TakeDamage(damage); 
            std::cout << "--- Assumed Damage: " << damage << " ---" << std::endl;
        }
    }
};


// ----------------------------------------------------------------------------
// 2. F_WindowController 核心实现
// ----------------------------------------------------------------------------

F_WindowController::F_WindowController() {
    // F_NetWork 应该在 FloatApi.cpp 中被初始化，这里只做指针的初始化
    m_net = std::make_unique<F_NetWork>();
    SetupProtocolHandlers();
}

F_WindowController::~F_WindowController() {
    if (m_net) m_net->Shutdown();
}

void F_WindowController::SetupProtocolHandlers() {
    m_protocolMap[WindowProtocolID::UPDATE_STATE] = std::make_unique<UpdateStateHandler>();
    m_protocolMap[WindowProtocolID::INTERACTION_REPORT] = std::make_unique<InteractionReportHandler>();
    // ... 其他协议 ...
}

std::string F_WindowController::GetLocalIPAddress() {
    // 实际项目中需要使用 winsock/ioctl/gethostbyname 等 API 来获取本地IP
    // 这里简化为 "127.0.0.1" (本地回环)
    return "127.0.0.1";
}

void F_WindowController::Start(int localPort) {
    if (!m_net->Initialize()) return;

    // 设置本地 ID 为 IP:Port (例如 127.0.0.1:8000)
    m_localID = GetLocalIPAddress() + ":" + std::to_string(localPort);

    // 启动本地 Socket 服务
    if (!m_net->CreateServer(localPort, 10)) {
        std::cerr << "Failed to start local net server." << std::endl;
        return;
    }

    // 初始化本地窗口状态
    WindowTarget localTarget = {
        m_localID,
        GetWindowPosition().x,
        GetWindowPosition().y,
        GetScreenWidth(),
        GetScreenHeight(),
        true,
        true
    };
    m_targets[m_localID] = localTarget;
    std::cout << "WindowController started with ID: " << m_localID << std::endl;
}

void F_WindowController::Update() {
    // 1. 处理所有网络事件
    ProcessNetworkEvents();

    // 2. 发送自己的状态 (例如每 5 帧发送一次)
    static int frameCount = 0;
    if (frameCount++ % 5 == 0) {
        SendLocalStateUpdate();
    }
}

void F_WindowController::ConnectToTarget(const std::string& ip, int port) {
    // F_NetWork 尝试建立连接 (TCP/UDP)
    m_net->ConnectToServer(ip.c_str(), port);
}

// ----------------------------------------------------------------------
// 内部通信和事件处理
// ----------------------------------------------------------------------

void F_WindowController::SendLocalStateUpdate() {
    // 1. 更新本地状态
    Vector2 pos = GetWindowPosition();
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    WindowTarget& localTarget = m_targets[m_localID];
    localTarget.x = pos.x;
    localTarget.y = pos.y;
    localTarget.width = w;
    localTarget.height = h;

    // 2. 构造数据包 (ID + x + y + w + h + active)
    std::vector<unsigned char> data;
    data.reserve(sizeof(WindowProtocolID) + 4 * sizeof(int) + sizeof(bool));

    WindowProtocolID id = WindowProtocolID::UPDATE_STATE;

    // 写入协议ID
    data.insert(data.end(), (unsigned char*)&id, (unsigned char*)&id + sizeof(WindowProtocolID));
    // 写入 x, y, w, h, active
    data.insert(data.end(), (unsigned char*)&localTarget.x, (unsigned char*)&localTarget.x + sizeof(int));
    data.insert(data.end(), (unsigned char*)&localTarget.y, (unsigned char*)&localTarget.y + sizeof(int));
    data.insert(data.end(), (unsigned char*)&localTarget.width, (unsigned char*)&localTarget.width + sizeof(int));
    data.insert(data.end(), (unsigned char*)&localTarget.height, (unsigned char*)&localTarget.height + sizeof(int));
    data.insert(data.end(), (unsigned char*)&localTarget.is_active, (unsigned char*)&localTarget.is_active + sizeof(bool));

    // 3. 广播给所有连接 (不可靠发送，因为是实时更新)
    m_net->BroadcastPacket(data.data(), data.size(), false);
}

void F_WindowController::ProcessNetworkEvents() {
    NetEvent evt;
    // 循环处理所有积压的网络事件
    while ((evt = m_net->Service()).type != NetEventType::NONE) {

        if (evt.type == NetEventType::RECEIVE) {
            if (evt.packetData.size() < sizeof(WindowProtocolID)) continue;

            WindowProtocolID protocolID;
            std::memcpy(&protocolID, evt.packetData.data(), sizeof(WindowProtocolID));

            // 查找策略并执行
            auto it = m_protocolMap.find(protocolID);
            if (it != m_protocolMap.end()) {
                // 将数据包的有效载荷部分 (ID 之后) 传给处理器
                std::vector<unsigned char> payload(
                    evt.packetData.begin() + sizeof(WindowProtocolID),
                    evt.packetData.end()
                );
                // 使用连接ID (假设 F_NetWork 已经处理了 ID 映射)
                std::string senderID = std::to_string(evt.connectionID);
                it->second->Handle(this, senderID, payload);
            }
        }
        // TODO: 处理 NetEventType::CONNECT/DISCONNECT 事件来增删 m_targets 列表
    }
}

// ----------------------------------------------------------------------
// 3. 外部接口实现 (供游戏主程序调用)
// ----------------------------------------------------------------------

void F_WindowController::UpdateRemoteState(const std::string& id, int x, int y, int w, int h, bool active) {
    if (id == m_localID) return; // 避免更新自己

    WindowTarget& target = m_targets[id];

    // 如果是新连接，初始化它
    if (target.id.empty()) {
        target.id = id;
        std::cout << "[Controller] New Window Target Discovered: " << id << std::endl;
    }

    target.x = x;
    target.y = y;
    target.width = w;
    target.height = h;
    target.is_active = active;

    // TODO: 通知所有观察者 (Observer Pattern)
}

bool F_WindowController::CheckBoundaryInteraction(rlRectangle localObjectRect, const std::vector<unsigned char>& interactionData) {
    bool hit = false;

    // 获取本地窗口的全局位置
    Vector2 localPos = GetWindowPosition();

    for (const auto& pair : m_targets) {
        const WindowTarget& target = pair.second;
        if (target.id == m_localID) continue; // 跳过自己
        if (!target.is_active) continue; // 跳过死亡目标

        // 1. 将本地对象坐标从本地窗口空间转换到桌面全局空间
        rlRectangle globalObject = {
            localPos.x + localObjectRect.x,
            localPos.y + localObjectRect.y,
            localObjectRect.width,
            localObjectRect.height
        };

        // 2. 目标窗口的全局矩形
        rlRectangle targetRect = {
            (float)target.x,
            (float)target.y,
            (float)target.width,
            (float)target.height
        };

        // 3. 检查全局碰撞
        if (CheckCollisionRecs(globalObject, targetRect)) {
            // 4. 如果击中，发送 INTERACTION_REPORT 包给目标窗口的服务器
            std::vector<unsigned char> data;
            data.reserve(sizeof(WindowProtocolID) + interactionData.size());

            WindowProtocolID id = WindowProtocolID::INTERACTION_REPORT;
            data.insert(data.end(), (unsigned char*)&id, (unsigned char*)&id + sizeof(WindowProtocolID));
            data.insert(data.end(), interactionData.begin(), interactionData.end()); // 写入自定义交互数据

            // 发送给目标窗口 (假设 F_NetWork 已经处理了 ID 映射)
            // m_net->SendPacket(target.connectionID, data.data(), data.size(), true); 
            std::cout << "[Controller] Interaction Reported to " << target.id << " for " << interactionData.size() << " bytes." << std::endl;
            hit = true;
        }
    }
    return hit;
}