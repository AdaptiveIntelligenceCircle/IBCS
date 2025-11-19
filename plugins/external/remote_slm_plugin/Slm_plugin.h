#pragma once 
#include <string> 
#include <vector> 

#include <memory> 
#include <optional> 

using namespace std; 

#include "base/plugin_interface.h"
#include "../plugin_interface.h"

namespace ibcs :: plugin :: external :: remote_slm
{
    struct InferenceResult
    {
        bool success = false; 
        string raw_response; 
        string error; 
    }; 

    class HttpClient;
    
    class RemoteSLMPlugin : public ibcs :: plugin :: IPlugin{
        public: 
        RemoteSLMPlugin(); 
        ~RemoteSLMPlugin(); 

        // IPlugin interface.. 
        ibcs :: plugin :: PluginInfo info() const override; 
        bool on_load() override; 
        bool on_unload() override; 

        vector<string> capabilities() const override ; 

        // configure endpoint + api key ; returns true if accepted.
        bool configure_endpoint(const string &url, const string &api_key); 

        // Synchronous text inference. Returns raw response ...
        InferenceResult infer(const string &prompt, int max_tokens = 100); 

        // simple health check / ping the endpoint ..
        InferenceResult health_check();

        private: 
        unique_ptr <HttpClient> m_http_client;
        string endpoint_url_; 
        string api_key_; 
        string default_model_; 
        int default_max_tokens_ = 256; 

        // helper to build request payload .. 
        string build_payload(const string &prompt, int max_token = 100); 
    }; 
}
extern "C"
{
    ibcs :: plugin :: IPlugin *create_plugin(); 
    void destroy_plugin(ibcs :: plugin :: IPlugin *p); 
}