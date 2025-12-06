#include "F_NetWork.h"
#include <iostream>
#include <cstring>
#include <algorithm>

// ----------------------------------------------------------------------------
// 引入 SLikeNet 头文件 (只在 .cpp 中)
// ----------------------------------------------------------------------------
#include "slikenet/peerinterface.h"
#include "slikenet/Peer.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/BitStream.h"
#include "slikenet/Types.h"
#include "slikenet/SocketDefines.h" 

using namespace SLNet;


// ----------------------------------------------------------------------------
// F_NetWork 实现
// ----------------------------------------------------------------------------

F_NetWork::F_NetWork() : m_impl(std::make_unique<Impl>()) {
    m_impl.get()->peer = (void*)RakPeerInterface::GetInstance();
}

F_NetWork::~F_NetWork() {
    Shutdown();
    if (m_impl->peer) {
        RakPeerInterface::DestroyInstance(static_cast<RakPeerInterface*>(m_impl->peer));
        m_impl->peer = nullptr;
    }
}

bool F_NetWork::Initialize() {
    return m_impl->peer != nullptr;
}

void F_NetWork::SetError(NetErrorCode code, const std::string& msg) {
    m_impl->lastError.code = code;
    m_impl->lastError.message = msg;
    std::cerr << "[SLikeNet Error] " << msg << std::endl;
}

// ----------------------------------------------------------------------------
// Server/Client Logic
// ----------------------------------------------------------------------------

bool F_NetWork::CreateServer(int port, int maxClients) {
    if (!m_impl->peer) return false;
    Shutdown();

    SocketDescriptor sd(port, 0);
    StartupResult res = static_cast<RakPeerInterface*>(m_impl.get()->peer)->Startup(maxClients, &sd, 1);

    if (res != RAKNET_STARTED) {
        SetError(NetErrorCode::STARTUP_FAILED, "Server Startup Failed. Error: " + std::to_string(res));
        return false;
    }

    static_cast<RakPeerInterface*>(m_impl.get()->peer)->SetMaximumIncomingConnections(maxClients);
    m_impl->mode = NetMode::SERVER;
    return true;
}

bool F_NetWork::ConnectToServer(const char* host, int port) {
    if (!m_impl->peer) return false;
    Shutdown();

    SocketDescriptor sd;
    StartupResult res = static_cast<RakPeerInterface*>(m_impl.get()->peer)->Startup(1, &sd, 1);

    if (res != RAKNET_STARTED) {
        SetError(NetErrorCode::STARTUP_FAILED, "Client Startup Failed. Error: " + std::to_string(res));
        return false;
    }

    ConnectionAttemptResult connRes = static_cast<RakPeerInterface*>(m_impl.get()->peer)->Connect(host, port, nullptr, 0);

    if (connRes != CONNECTION_ATTEMPT_STARTED) {
        SetError(NetErrorCode::CONNECTION_FAILED, "Connection Attempt Failed. Error: " + std::to_string(connRes));
        return false;
    }

    m_impl->mode = NetMode::CLIENT;
    return true;
}

void F_NetWork::Shutdown() {
    if (m_impl->peer && static_cast<RakPeerInterface*>(m_impl.get()->peer)->IsActive()) {
        static_cast<RakPeerInterface*>(m_impl.get()->peer)->Shutdown(300);
    }
    m_impl->mode = NetMode::NONE;

    std::queue<NetEvent> empty;
    std::swap(m_impl->eventQueue, empty);
    m_impl->lastError = NetError();
}

// ----------------------------------------------------------------------------
// Send Logic
// ----------------------------------------------------------------------------

void F_NetWork::SendPacket(uint64_t targetID, const void* data, size_t dataLength, bool reliable) {
    if (!m_impl->peer || m_impl->mode == NetMode::NONE) return;

    BitStream bs;
    bs.Write((unsigned char)ID_FLOAT_ENGINE_DATA);
    bs.Write((const char*)data, (unsigned int)dataLength);

    PacketReliability reliabilityType = reliable ? RELIABLE_ORDERED : UNRELIABLE;

    AddressOrGUID target;
    bool broadcast = false;

    if (m_impl->mode == NetMode::CLIENT) {
        // 客户端发给服务器 (UNASSIGNED_SYSTEM_ADDRESS)
        target = UNASSIGNED_SYSTEM_ADDRESS;
        broadcast = true; // 对客户端来说，对 UNASSIGNED_SYSTEM_ADDRESS 广播就是发送给服务器
    }
    else {
        // 服务器发给指定 ID
        if (targetID == 0) return; // 无效 ID
        target.rakNetGuid.g = targetID;
        broadcast = false;
    }

    static_cast<RakPeerInterface*>(m_impl.get()->peer)->Send(&bs, HIGH_PRIORITY, reliabilityType, 0, target, broadcast);
}

void F_NetWork::BroadcastPacket(const void* data, size_t dataLength, bool reliable) {
    if (!m_impl->peer || m_impl->mode != NetMode::SERVER) return;

    BitStream bs;
    bs.Write((unsigned char)ID_FLOAT_ENGINE_DATA);
    bs.Write((const char*)data, (unsigned int)dataLength);

    PacketReliability reliabilityType = reliable ? RELIABLE_ORDERED : UNRELIABLE;

    static_cast<RakPeerInterface*>(m_impl.get()->peer)->Send(&bs, HIGH_PRIORITY, reliabilityType, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

// ----------------------------------------------------------------------------
// Service Logic
// ----------------------------------------------------------------------------

NetEvent F_NetWork::Service() {
    NetEvent result;

    // 1. 优先返回队列中积压的事件
    if (!m_impl->eventQueue.empty()) {
        result = m_impl->eventQueue.front();
        m_impl->eventQueue.pop();
        return result;
    }

    if (!m_impl->peer) return result;

    // 2. 从 RakNet 接收新数据包
    Packet* packet;

    for (packet = static_cast<RakPeerInterface*>(m_impl.get()->peer)->Receive(); packet; static_cast<RakPeerInterface*>(m_impl.get()->peer)->DeallocatePacket(packet), packet = static_cast<RakPeerInterface*>(m_impl.get()->peer)->Receive()) {

        uint64_t guid = packet->guid.g;
        unsigned char packetIdentifier = (unsigned char)packet->data[0];

        // 处理时间戳偏移
        if (packetIdentifier == ID_TIMESTAMP) {
            if (packet->length > sizeof(MessageID) + sizeof(Time)) {
                packetIdentifier = (unsigned char)packet->data[sizeof(MessageID) + sizeof(Time)];
            }
        }

        NetEvent evt = { NetEventType::NONE, 0, {}, "" };
        evt.connectionID = guid;

        // 3. 根据 ID 分类处理
        switch (packetIdentifier) {
        case ID_CONNECTION_REQUEST_ACCEPTED: // 客户端连接成功
        case ID_NEW_INCOMING_CONNECTION:     // 服务器收到新连接
            evt.type = NetEventType::CONNECT;
            evt.message = (packetIdentifier == ID_CONNECTION_REQUEST_ACCEPTED) ? "Connected to Server" : "New Client: " + std::string(packet->systemAddress.ToString(false));
            m_impl->eventQueue.push(evt);
            break;

        case ID_DISCONNECTION_NOTIFICATION:
        case ID_CONNECTION_LOST:
            evt.type = NetEventType::DISCONNECT;
            evt.message = "Disconnected";
            m_impl->eventQueue.push(evt);
            break;

        case ID_FLOAT_ENGINE_DATA: // 收到游戏数据
        {
            evt.type = NetEventType::RECEIVE;

            // 跳过协议头 (1 字节) 或 (1 字节 ID + 4 字节 Time)
            unsigned int offset = (packet->data[0] == ID_TIMESTAMP) ? (sizeof(MessageID) + sizeof(Time) + 1) : 1;

            if (packet->length > offset) {
                evt.packetData.assign(packet->data + offset, packet->data + packet->length);
                m_impl->eventQueue.push(evt);
            }
            break;
        }

        default:
            // 忽略其他系统包
            break;
        }
    }

    // 3. 返回队列中的事件 (如果 Service 循环中产生了新的事件)
    if (!m_impl->eventQueue.empty()) {
        result = m_impl->eventQueue.front();
        m_impl->eventQueue.pop();
    }

    return result;
}