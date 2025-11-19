#include "Slm_plugin.h"
#include "http_client.h"

#include <iostream>
#include <sstream> 

using namespace std; 
#include "../utils/mini_json.hpp"

namespace ibcs :: plugin :: external :: remote_slm
{
    using namespace MiniJson; 

    RemoteSLMPlugin :: RemoteSLMPlugin()
    : m_http_client(make_unique<HttpClient>())
    {

    }

    RemoteSLMPlugin :: ~RemoteSLMPlugin()
    {
        // cleanup.. 
    }

    ibcs ::plugin :: PluginInfo RemoteSLMPlugin :: info() const 
    {
        return {
            "external.remote_slm", "1.0.0", "IBCS External Team"
        }; 
    }

    bool RemoteSLMPlugin :: on_load()
    {
        cout <<"Remote slm - plugin loaded" << endl; 
        return true; 
    }

    bool RemoteSLMPlugin :: on_unload()
    {
        cout <<"Unloaded" << endl; 
        return true; 
    }

    vector<string> RemoteSLMPlugin :: capabilities() const 
    {
        return {"external.remote_slm"
        , "external.health_check"}; 
    }

    bool RemoteSLMPlugin::configure_endpoint(const string &url, const string &api_key) 
    {
    if (url.empty()) return false;
    endpoint_url_ = url;
    api_key_ = api_key;
    // optional quick ping
    auto r = health_check();
    if (!r.success) {
        cerr << "[remote_slm] health_check failed during configure: " << r.error << "\n";
        // still accept config but report issue
    }
    return true;
    }

    string RemoteSLMPlugin :: build_payload(const string &prompt, int max_token)
    {
        ostringstream oss; 
        oss << "{" ; 
        oss << "\"prompt\":\"" << prompt << "\"" << endl; 

        // naive json string escape.. for prompt
        oss << "\"" << endl;
        
        for (char c : prompt){
            if (c == '\\') oss << "\\\\"; 
            else if (c== '"') oss << "\\\""; 
            else if (c == '\n') oss << "\\n";
            else oss << c;
        }
        oss << "\"";
    // if (!model.empty()) oss << ",\"model\":\"" << model << "\"";
    oss << ",\"max_tokens\":" << max_token;
    oss << "}";
    return oss.str();
    }

    InferenceResult RemoteSLMPlugin::infer(const string &prompt, int max_tokens, const string &model) {
    InferenceResult out;
    if (endpoint_url_.empty()) {
        out.success = false;
        out.error = "endpoint_not_configured";
        return out;
    }
    int mt = (max_tokens <= 0) ? default_max_tokens_ : max_tokens;
    string payload = build_payload(prompt, mt, model.empty() ? default_model_ : model);

    map<string,string> headers;
    headers["Content-Type"] = "application/json";
    if (!api_key_.empty()) headers["Authorization"] = "Bearer " + api_key_;

    auto resp = m_http_client->post(endpoint_url_, headers, payload);
    if (!resp.first) {
        out.success = false;
        out.error = resp.second;
        return out;
    }
    out.success = true;
    out.raw_response = resp.second;

    // Optionally attempt to parse JSON and extract "text" or "choices[0].text"
    try {
        Value v = parse_string(out.raw_response);
        if (v.is_object()) {
            if (v.contains("text") && v["text"].is_string()) {
                out.raw_response = v["text"].as_string();
            } else if (v.contains("choices") && v["choices"].is_array()) {
                auto &arr = v["choices"].as_array();
                if (!arr.empty() && arr[0].is_object() && arr[0].contains("text")) {
                    out.raw_response = arr[0]["text"].as_string();
                }
            }
        }
    } catch (...) {
        // parse failure: keep raw_response as-is
    }

    return out;
}

InferenceResult RemoteSLMPlugin::health_check() 
{
    InferenceResult out;
    if (endpoint_url_.empty()) 
    {
        out.success = false;
        out.error = "endpoint_not_configured";
        return out;
    }

    // Try GET on endpoint root (some endpoints reject; best-effort)
    map<string,string> headers;
    if (!api_key_.empty()) headers["Authorization"] = "Bearer " + api_key_;
    auto resp = m_http_client->get(endpoint_url_, headers);
    if (!resp.first) {
        // Try a lightweight /health or /ping path
        string ping_url = endpoint_url_;
        if (endpoint_url_.back() == '/') ping_url += "health";
        else ping_url += "/health";
        auto r2 = m_http_client->get(ping_url, headers);
        if (!r2.first) {
            out.success = false;
            out.error = resp.second + " | " + r2.second;
            return out;
        }
        out.success = true;
        out.raw_response = r2.second;
        return out;
    }
    out.success = true;
    out.raw_response = resp.second;
    return out;
}

// exported symbols for loader
extern "C" ibcs::plugin::IPlugin* create_plugin() 
{
    return new RemoteSLMPlugin();
}

extern "C" void destroy_plugin(ibcs::plugin::IPlugin* p) 
{
    delete p;
}

}