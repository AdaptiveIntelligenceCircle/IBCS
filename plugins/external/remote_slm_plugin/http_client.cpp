#include "http_client.h"
#include <iostream> 

using namespace std; 
#include <sstream> 

#if defined(__has_include)
  #if __has_include(<curl/curl.h>)
    #define REMOTE_SLM_HAVE_CURL 1
    #include <curl/curl.h>
  #else
    #define REMOTE_SLM_HAVE_CURL 0
  #endif
#else
  #define REMOTE_SLM_HAVE_CURL 0
#endif

namespace ibcs :: plugin :: external :: remote_slm
{
    HttpClient::HttpClient() : timeout_seconds_(10) {
#if REMOTE_SLM_HAVE_CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
#endif
}

HttpClient::~HttpClient() {
#if REMOTE_SLM_HAVE_CURL
    curl_global_cleanup();
#endif
}

void HttpClient::set_timeout(int seconds) {
    timeout_seconds_ = seconds;
}

#if REMOTE_SLM_HAVE_CURL

// helper for response write
static size_t curl_write_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string* out = reinterpret_cast<string*>(userdata);
    size_t total = size * nmemb;
    out->append(ptr, total);
    return total;
}

pair<bool, string> HttpClient::post(const string &url,
                                              const map<string, string> &headers,
                                              const string &body) {
    CURL* curl = curl_easy_init();
    if (!curl) return {false, "curl_init_failed"};

    struct curl_slist* chunk = nullptr;
    for (auto &h : headers) {
        string hh = h.first + ": " + h.second;
        chunk = curl_slist_append(chunk, hh.c_str());
    }

    string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout_seconds_);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)body.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    if (chunk) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    CURLcode res = curl_easy_perform(curl);
    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

    if (chunk) curl_slist_free_all(chunk);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        ostringstream ss;
        ss << "curl_error: " << curl_easy_strerror(res);
        return {false, ss.str()};
    }
    if (code >= 200 && code < 300) return {true, response};
    ostringstream ss;
    ss << "http_status=" << code << " body=" << response;
    return {false, ss.str()};
}

pair<bool, string> HttpClient::get(const string &url,
                                             const map<string, string> &headers) {
    CURL* curl = curl_easy_init();
    if (!curl) return {false, "curl_init_failed"};

    struct curl_slist* chunk = nullptr;
    for (auto &h : headers) {
        string hh = h.first + ": " + h.second;
        chunk = curl_slist_append(chunk, hh.c_str());
    }

    string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout_seconds_);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    if (chunk) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    CURLcode res = curl_easy_perform(curl);
    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

    if (chunk) curl_slist_free_all(chunk);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        ostringstream ss;
        ss << "curl_error: " << curl_easy_strerror(res);
        return {false, ss.str()};
    }
    if (code >= 200 && code < 300) return {true, response};
    ostringstream ss;
    ss << "http_status=" << code << " body=" << response;
    return {false, ss.str()};
}

#else // no curl available -> provide clear stub that returns error

pair<bool, string> HttpClient::post(const string &url,
                                              const map<string, string> &headers,
                                              const string &body) {
    (void)headers; (void)body;
    ostringstream ss;
    ss << "http_client_stub: libcurl not available. Cannot POST to " << url;
    cerr << ss.str() << "\n";
    return {false, ss.str()};
}

pair<bool, string> HttpClient::get(const string &url,
                                             const map<string, string> &headers) {
    (void)headers;
    ostringstream ss;
    ss << "http_client_stub: libcurl not available. Cannot GET " << url;
    cerr << ss.str() << "\n";
    return {false, ss.str()};
}

#endif
}