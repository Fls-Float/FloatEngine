#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>

// ----------------------------------------------------------------------------
// 内部前置声明
// ----------------------------------------------------------------------------
class AsyncWorker;

/**
 * @class F_Http
 * @brief 封装 Libcurl，实现高效同步和异步 HTTP/HTTPS 功能
 */
class F_Http {
public:
    // ------------------------------------------------------------------------
    // 基础数据结构
    // ------------------------------------------------------------------------
    using HttpHeaders = std::map<std::string, std::string>;
    using AsyncCallback = std::function<void(bool success, int httpCode, const std::string& responseBody)>;

    struct RequestResult {
        bool success = false;
        int httpCode = 0;
        std::string responseBody;
        std::string errorMessage;
    };

    F_Http();
    ~F_Http();

    // 禁用拷贝，使用默认移动
    F_Http(const F_Http&) = delete;
    F_Http& operator=(const F_Http&) = delete;
    F_Http(F_Http&&) = default;
    F_Http& operator=(F_Http&&) = default;

    // ------------------------------------------------------------------------
    // 同步 (阻塞) API
    // ------------------------------------------------------------------------
    RequestResult Get(const std::string& url, const HttpHeaders& headers = {});
    RequestResult Post(const std::string& url, const std::string& postData, const HttpHeaders& headers = {});
    RequestResult PostJson(const std::string& url, const std::string& jsonBody, const HttpHeaders& additionalHeaders = {});

    // ------------------------------------------------------------------------
    // 异步 (CURLM) API
    // ------------------------------------------------------------------------
    void GetAsync(const std::string& url, const HttpHeaders& headers, AsyncCallback callback);
    void PostAsync(const std::string& url, const std::string& postData, const HttpHeaders& headers, AsyncCallback callback);

private:
    // 异步管理器 (PIMPL)
    std::unique_ptr<AsyncWorker> m_asyncWorker;

    // 内部回调函数
    static size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp);
    static size_t HeaderCallback(char* buffer, size_t size, size_t nitems, void* userp);

    // 内部执行请求的通用函数
    RequestResult ExecuteRequest(void* curl, const std::string& url, const HttpHeaders& headers, const std::string& postData = "", bool isPost = false);
};

// ----------------------------------------------------------------------------
// 内部异步工作线程类 (AsyncWorker)
// ----------------------------------------------------------------------------
struct AsyncRequestData {
    void* easy_handle = nullptr;
    F_Http::AsyncCallback callback;
    std::string responseBody;
};

class AsyncWorker {
public:
    AsyncWorker();
    ~AsyncWorker();

    // 添加请求到 multi 句柄
    void AddRequest(void* easy_handle, F_Http::AsyncCallback callback);
    void Stop();

    // 供 WriteCallback 访问的请求数据
    std::map<void*, std::unique_ptr<AsyncRequestData>> m_requests;

private:
    void* m_multiHandle = nullptr;
    std::thread m_workerThread;
    std::atomic<bool> m_running{ false };

    // 线程主循环
    void WorkerLoop();
};