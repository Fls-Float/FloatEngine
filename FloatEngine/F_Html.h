#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <thread>

/**
 * @class F_Html
 * @brief 封装 Libcurl，实现 HTTP/HTTPS 的 GET/POST 和高级功能
 */
class F_Html {
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

    F_Html();
    ~F_Html();

    // 禁用拷贝/移动，因为 Libcurl 句柄是资源
    F_Html(const F_Html&) = delete;
    F_Html& operator=(const F_Html&) = delete;

    // ------------------------------------------------------------------------
    // 同步 (阻塞) API
    // ------------------------------------------------------------------------

    /**
     * @brief 同步 GET 请求 (会阻塞当前线程)
     */
    RequestResult Get(const std::string& url, const HttpHeaders& headers = {});

    /**
     * @brief 同步 POST 请求 (会阻塞当前线程)
     * @param postData 要发送的数据体 (如 JSON 或表单数据)
     */
    RequestResult Post(const std::string& url, const std::string& postData, const HttpHeaders& headers = {});

    // ------------------------------------------------------------------------
    // 异步 API
    // ------------------------------------------------------------------------

    /**
     * @brief 异步 GET 请求 (在新线程中执行)
     * @param callback 请求完成后的回调函数
     */
    void GetAsync(const std::string& url, const HttpHeaders& headers, AsyncCallback callback);

    /**
     * @brief 异步 POST 请求 (在新线程中执行)
     * @param postData 要发送的数据体
     */
    void PostAsync(const std::string& url, const std::string& postData, const HttpHeaders& headers, AsyncCallback callback);

    /**
     * @brief 同步 POST JSON (自动设置 Content-Type: application/json)
     */
    RequestResult PostJson(const std::string& url, const std::string& jsonBody, const HttpHeaders& additionalHeaders = {});

private:
    // 内部写回调函数，用于 Libcurl 接收数据
    static size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp);

    // 内部写回调函数，用于 Libcurl 接收 Header
    static size_t HeaderCallback(char* buffer, size_t size, size_t nitems, void* userp);

    // 内部执行请求的通用函数
    RequestResult ExecuteRequest(void* curl, const std::string& url, const HttpHeaders& headers, const std::string& postData = "", bool isPost = false);
};