#include "F_Http.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <curl/curl.h> 
// ----------------------------------------------------------------------------
// 1. Libcurl 内部回调函数实现
// ----------------------------------------------------------------------------

// 数据写入回调 (用于接收 Response Body)
size_t F_Http::WriteCallback(char* contents, size_t size, size_t nmemb, void* userp) {
    // userp 是 AsyncRequestData* 或 std::string* (取决于同步/异步)
    if (userp) {
        ((std::string*)userp)->append(contents, size * nmemb);
    }
    return size * nmemb;
}

// Header 写入回调 (通常忽略，但必须提供)
size_t F_Http::HeaderCallback(char* buffer, size_t size, size_t nitems, void* userp) {
    return size * nitems;
}

// ----------------------------------------------------------------------------
// 2. AsyncWorker 实现 (CURLM 主循环)
// ----------------------------------------------------------------------------

AsyncWorker::AsyncWorker() {
    // 全局初始化 Libcurl (推荐在 main 函数中调用 curl_global_init)
    // 这里是为了确保初始化，但最好只调用一次
    curl_global_init(CURL_GLOBAL_ALL);

    m_multiHandle = curl_multi_init();
    if (!m_multiHandle) {
        throw std::runtime_error("CURLM initialization failed.");
    }
    m_running = true;
    m_workerThread = std::thread(&AsyncWorker::WorkerLoop, this);
}

AsyncWorker::~AsyncWorker() {
    Stop();
    // 析构时清理全局 Libcurl (如果 main 没有做，这里做)
    // curl_global_cleanup(); 
}

void AsyncWorker::Stop() {
    if (m_running.exchange(false)) {
        // 中断并清理 worker 线程
        if (m_multiHandle) {
            // 设置一个短超时，唤醒 curl_multi_wait
            curl_multi_timeout(m_multiHandle, 0L);
        }
        if (m_workerThread.joinable()) {
            m_workerThread.join();
        }
    }

    // 清理 multi handle 和所有 easy handles
    if (m_multiHandle) {
        for (auto const& [handle, req] : m_requests) {
            curl_multi_remove_handle(m_multiHandle, handle);
            curl_easy_cleanup(handle);
        }
        m_requests.clear();
        curl_multi_cleanup(m_multiHandle);
        m_multiHandle = nullptr;
    }
}

void AsyncWorker::AddRequest(void* easy_handle, F_Http::AsyncCallback callback) {
    if (!m_running) {
        curl_easy_cleanup(easy_handle);
        return;
    }

    auto req = std::make_unique<AsyncRequestData>();
    req->easy_handle = easy_handle;
    req->callback = std::move(callback);

    // 设置回调目标为请求的 responseBody
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &req->responseBody);

    m_requests[easy_handle] = std::move(req);
    curl_multi_add_handle(m_multiHandle, easy_handle);
}

void AsyncWorker::WorkerLoop() {
    int still_running = 1;

    while (m_running) {
        // 1. 执行 Multi 句柄
        CURLMcode mc = curl_multi_perform(m_multiHandle, &still_running);
        if (mc != CURLM_OK) break;

        // 2. 检查已完成的请求
        int msgs_in_queue;
        CURLMsg* msg = curl_multi_info_read(m_multiHandle, &msgs_in_queue);

        if (msg) {
            for (; msg != nullptr; msg = curl_multi_info_read(m_multiHandle, &msgs_in_queue)) {
                if (msg->msg == CURLMSG_DONE) {
                    CURL* easy_handle = msg->easy_handle;
                    auto it = m_requests.find(easy_handle);

                    if (it != m_requests.end()) {
                        AsyncRequestData* req = it->second.get();

                        // 获取结果
                        long http_code = 0;
                        curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &http_code);

                        // 触发回调
                        bool success = (msg->data.result == CURLE_OK && http_code >= 200 && http_code < 300);
                        req->callback(success, (int)http_code, req->responseBody);

                        // 清理
                        curl_multi_remove_handle(m_multiHandle, easy_handle);
                        curl_easy_cleanup(easy_handle);
                        m_requests.erase(it);
                    }
                }
            }
        }

        // 3. 等待 I/O (非阻塞地等待活动)
        if (still_running) {
            long timeout_ms = 1000;
            curl_multi_timeout(m_multiHandle, &timeout_ms);
            if (timeout_ms < 0) timeout_ms = 1000;

            int numfds;
            curl_multi_wait(m_multiHandle, nullptr, 0, timeout_ms, &numfds);
        }
        else {
            // 没有活动连接时，短暂休眠
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

// ----------------------------------------------------------------------------
// 3. F_Http 接口实现
// ----------------------------------------------------------------------------

F_Http::F_Http() : m_asyncWorker(std::make_unique<AsyncWorker>()) {}

F_Http::~F_Http() {
    // 析构时停止 worker 线程
    m_asyncWorker->Stop();
}

F_Http::RequestResult F_Http::ExecuteRequest(void* curl, const std::string& url, const HttpHeaders& headers, const std::string& postData, bool isPost) {
    RequestResult result;

    // 设置 URL, WriteFunction, FollowLocation, SSL等通用选项
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    if (isPost) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postData.length());
    }
    else {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }

    // 处理 Headers
    struct curl_slist* list = nullptr;
    for (const auto& header : headers) {
        std::string header_line = header.first + ": " + header.second;
        list = curl_slist_append(list, header_line.c_str());
    }
    if (list) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    }

    return result;
}


// -------------------- 同步 API --------------------

F_Http::RequestResult F_Http::Get(const std::string& url, const HttpHeaders& headers) {
    CURL* curl = curl_easy_init();
    if (!curl) return { false, 0, "", "Libcurl init failed" };

    RequestResult result;
    std::string responseBody;

    // 设置回调目标为局部变量 responseBody
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

    // 设置所有请求参数 (使用 ExecuteRequest 的逻辑)
    result = ExecuteRequest(curl, url, headers, "", false);

    // 执行同步请求
    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        result.success = true;
        result.httpCode = (int)http_code;
        result.responseBody = responseBody;
    }
    else {
        result.success = false;
        result.errorMessage = curl_easy_strerror(res);
    }

    curl_easy_cleanup(curl);
    return result;
}

F_Http::RequestResult F_Http::Post(const std::string& url, const std::string& postData, const HttpHeaders& headers) {
    CURL* curl = curl_easy_init();
    if (!curl) return { false, 0, "", "Libcurl init failed" };

    RequestResult result;
    std::string responseBody;

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

    result = ExecuteRequest(curl, url, headers, postData, true);

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        result.success = true;
        result.httpCode = (int)http_code;
        result.responseBody = responseBody;
    }
    else {
        result.success = false;
        result.errorMessage = curl_easy_strerror(res);
    }

    curl_easy_cleanup(curl);
    return result;
}

F_Http::RequestResult F_Http::PostJson(const std::string& url, const std::string& jsonBody, const HttpHeaders& additionalHeaders) {
    HttpHeaders headers = additionalHeaders;
    headers["Content-Type"] = "application/json";

    return Post(url, jsonBody, headers);
}

// -------------------- 异步 API --------------------

void F_Http::GetAsync(const std::string& url, const HttpHeaders& headers, AsyncCallback callback) {
    CURL* curl = curl_easy_init();
    if (!curl) { callback(false, 0, "Libcurl init failed"); return; }

    // 设置所有请求参数 (使用 ExecuteRequest 的逻辑)
    ExecuteRequest(curl, url, headers, "", false);

    m_asyncWorker->AddRequest(curl, callback);
}

void F_Http::PostAsync(const std::string& url, const std::string& postData, const HttpHeaders& headers, AsyncCallback callback) {
    CURL* curl = curl_easy_init();
    if (!curl) { callback(false, 0, "Libcurl init failed"); return; }

    ExecuteRequest(curl, url, headers, postData, true);

    m_asyncWorker->AddRequest(curl, callback);
}