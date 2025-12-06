#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>

// ----------------------------------------------------------------------------
// SLikeNet PIMPL 前置声明 (无需包含任何头文件)
// ----------------------------------------------------------------------------
namespace SLNet { class RakPeerInterface; } // 前置声明 SLikeNet 的核心接口

// ----------------------------------------------------------------------------
// 基础数据结构定义
// ----------------------------------------------------------------------------

enum class NetEventType {
    NONE, CONNECT, DISCONNECT, RECEIVE
};

enum class NetMode {
    NONE, SERVER, CLIENT
};

enum class NetErrorCode {
    NONE = 0, STARTUP_FAILED, CONNECTION_FAILED, SEND_FAILED
};

struct NetError {
    NetErrorCode code = NetErrorCode::NONE;
    std::string message;
};

struct NetEvent {
    NetEventType type = NetEventType::NONE;
    uint64_t connectionID = 0;             // 唯一连接标识 (RakNetGUID.g)
    std::vector<unsigned char> packetData;
    std::string message;                   // 附带信息
};

/* 
 *----------------------------------------------------------------------------
 * F_NetWork 类定义
 *----------------------------------------------------------------------------
*/
class F_NetWork {
public:
    F_NetWork();
    ~F_NetWork();

    F_NetWork(const F_NetWork&) = delete;
    F_NetWork& operator=(const F_NetWork&) = delete;

    bool Initialize();
    bool CreateServer(int port, int maxClients = 32);
    bool ConnectToServer(const char* host, int port);
    void Shutdown();

    // targetID: 目标连接ID。0 广播/发服务器。
    void SendPacket(uint64_t targetID, const void* data, size_t dataLength, bool reliable = true);
    void BroadcastPacket(const void* data, size_t dataLength, bool reliable = true);

    NetEvent Service();

    NetMode GetMode() const { return m_impl.get()->mode; }
    const NetError& GetLastError() const { return m_impl.get()->lastError; }

private:
    struct Impl {
        void* peer = nullptr;
        NetMode mode = NetMode::NONE;
        NetError lastError;
        std::queue<NetEvent> eventQueue;
    };
    std::unique_ptr<Impl> m_impl;
    void SetError(NetErrorCode code, const std::string& msg);

    // 自定义协议头 (在 .cpp 中使用)
    static const unsigned char ID_FLOAT_ENGINE_DATA = 135;
};