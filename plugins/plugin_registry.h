#pragma once 
#include "base/plugin_interface.h"
#include "plugin_manifest.h"
#include <string> 

#include <unordered_map>
#include <memory> 

using namespace std; 

namespace ibcs :: plugin
{
    struct LoadedPlugin
    {
        PluginManifest manifest; 
        // unique_ptr<PluginSandbox> sandbox; 
        PluginPtr instance = nullptr; 
        // handle for native loader 
        void * native_handler = nullptr; 
        string loaded_from; 
    }; 

    class PluginRegistry
    {
        public: 
        static PluginRegistry &instance(); 

        bool register_plugin(const string &id, LoadedPlugin &plugin); 
        bool unregister_plugin(const string &id); 
        bool has(const string &id) const; 
        LoadedPlugin *get(const string &id); 

        private: 
        PluginRegistry() = default; 
        unordered_map<string, LoadedPlugin> map_ ; 
    }; 
} // namespace ibcs :: plugin. . 
