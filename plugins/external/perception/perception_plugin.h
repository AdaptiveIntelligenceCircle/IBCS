#pragma once 
#include <iostream> 
#include <string> 

#include <vector> 
#include <memory>

#include <optional> 
#include <map> 

using namespace std; 

#include "base/plugin_interface.h"
#include "../plugin_interface.h"

namespace ibcs :: plugins :: external :: perception
{
    // simple detection result for demonstration.. //
    struct Detection 
    {
        string label;
        float confidence ; 
        float x = 0, y =0, w = 0, h = 0; 
    }; 

    struct PerceptionInput
    {
        vector<uint8_t> image_data; 
        string mime_type = "image/raw"; 
        map<string, string> meta; 
    }; 

    struct PerceptionResult
    {
        bool success = false; 
        string model_used; 
        vector<Detection> detections; 
        string error; 
    }; 

    // forward declaration ..// 
    class ModelLoader; 

    class PerceptionPlugin : public ibcs :: plugin :: IPlugin
    {
        public: 
        PerceptionPlugin();
        ~PerceptionPlugin(); 
        // IPlugin interface.. 
        ibcs :: plugin :: PluginInfo info() const override; 
        bool on_load() override; 
        bool on_unload() override; 
        vector<string> capabilities() const override; 

        // perception specific API.. 
        void load_model(const string &model_path, const string &model_id = "" ); 

        // unlodd model.. 
        bool unload_model(const string &model_id); 

        // list loaded models .. 
        vector<string> list_models() const; 

        // run inference 
        PerceptionResult run_inference(const PerceptionInput &in, const string &model_id = "");

        private: 
        unique_ptr<ModelLoader> loader_; 

        map<string, string> models_; 
        string default_model_id_; 

        // helper - pick model id or default .. 
        string resolve_model(const string &model_id) const; 
    }; 
}

extern "C"
{
    ibcs :: plugin :: IPlugin* create_plugin();
    void destroy_plugin(ibcs :: plugin :: IPlugin* plugin);
}
