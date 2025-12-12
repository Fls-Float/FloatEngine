#include "F_Html.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include <curl/curl.h> 
// ------------------------------------------------------------------------
// 1. Libcurl 内部回调函数实现
// ------------------------------------------------------------------------

// 数据写入回调 (用于接收 Response Body)
size_t F_Html::WriteCallback(char* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append(contents, size * nmemb);
    return size * nmemb;
}

// Header 写入回调 (用于接收 Response Headers)
size_t F_Html::HeaderCallback(char* buffer, size_t size, size_t nitems, void* userp) {
    // 可以在这里解析 header，但通常只需要 body，这里直接返回
    return size * nitems;
}

// ------------------------------------------------------------------------
// 2. 构造/析构
// ------------------------------------------------------------------------

F_Html::F_Html() {
    // 构造函数中不需要执行 curl_global_init，Libcurl 推荐只执行一次
}

F_Html::~F_Html() {
    // 析构函数中不需要执行 curl_global_cleanup
}

// ------------------------------------------------------------------------
// 3. 通用执行请求函数
// ------------------------------------------------------------------------

F_Html::RequestResult F_Html::ExecuteRequest(void* curl, const std::string& url, const HttpHeaders& headers, const std::string& postData, bool isPost) {
    RequestResult result;
    std::string responseBody;

    // 设置 URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // 设置数据写入回调和目标字符串
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

    // 允许重定向
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // 设置 SSL 认证 (Libcurl 默认是安全的，这里只是确保)
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    // 1. 处理 POST/GET
    if (isPost) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postData.length());
    }
    else {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }

    // 2. 处理 Headers
    struct curl_slist* list = nullptr;
    for (const auto& header : headers) {
        std::string header_line = header.first + ": " + header.second;
        list = curl_slist_append(list, header_line.c_str());
    }
    if (list) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    }

    // 3. 执行请求 (阻塞)
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

    // 清理 Header 列表
    if (list) {
        curl_slist_free_all(list);
    }

    return result;
}

// ------------------------------------------------------------------------
// 4. 同步 API 实现
// ------------------------------------------------------------------------

F_Html::RequestResult F_Html::Get(const std::string& url, const HttpHeaders& headers) {
    CURL* curl = curl_easy_init();
    if (!curl) return { false, 0, "", "Libcurl init failed" };

    RequestResult result = ExecuteRequest(curl, url, headers, "", false);

    curl_easy_cleanup(curl);
    return result;
}

F_Html::RequestResult F_Html::Post(const std::string& url, const std::string& postData, const HttpHeaders& headers) {
    CURL* curl = curl_easy_init();
    if (!curl) return { false, 0, "", "Libcurl init failed" };

    RequestResult result = ExecuteRequest(curl, url, headers, postData, true);

    curl_easy_cleanup(curl);
    return result;
}

// ------------------------------------------------------------------------
// 5. 异步 API 实现
// ------------------------------------------------------------------------

void F_Html::GetAsync(const std::string& url, const HttpHeaders& headers, AsyncCallback callback) {
    // 在新线程中执行同步请求
    std::thread([this, url, headers, callback]() {
        RequestResult result = this->Get(url, headers);
        callback(result.success, result.httpCode, result.responseBody);
        }).detach(); // 使用 detach 确保线程在函数结束后自动释放资源
}

void F_Html::PostAsync(const std::string& url, const std::string& postData, const HttpHeaders& headers, AsyncCallback callback) {
    // 在新线程中执行同步请求
    std::thread([this, url, postData, headers, callback]() {
        RequestResult result = this->Post(url, postData, headers);
        callback(result.success, result.httpCode, result.responseBody);
        }).detach();
}

// ------------------------------------------------------------------------
// 6. 实用扩展 (JSON Helper)
// ------------------------------------------------------------------------

F_Html::RequestResult F_Html::PostJson(const std::string& url, const std::string& jsonBody, const HttpHeaders& additionalHeaders) {
    // 自动设置 Content-Type: application/json
    HttpHeaders headers = additionalHeaders;
    headers["Content-Type"] = "application/json";

    return Post(url, jsonBody, headers);
}