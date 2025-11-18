#pragma once 

#include "plugin_manifest.h"
#include "base/plugin_interface.h"

#include "plugin_registry.h"
#include "base/plugin_sandbox.h"

#include <string> 
#include <memory> 

#include <vector> 
using namespace std ; 

namespace ibcs :: plugin{
    class PluginManager
    {
        public: 
        PluginManager(); 
        ~PluginManager(); 

        // load plugin manifest from file(json) and attempt to load plugin.. 
        bool load_plugin_from_file(const string &manifest_path); 

        // unload by plugin id (manifest.name)..// 
        bool unload_plugin(const string &plugin_id);

        // list loaded plugin ids 
        vector<string> list_plugin() const; 

        // QUery capabilities across plugins..//
        vector<string> find_plugins_with_capabilities(const string &cap) const; 

        private: 
        bool load_native(const PluginManifest &m); 
        bool load_lua(const PluginManifest &m); 
        bool load_wasm(const PluginManifest &m); 

        SandboxPolicy default_policy_for(const PluginManifest &m) const; 
    }; 

    unique_ptr<PluginManager> make_plugin_manager(); 
} // namespace ibcs :: plugin.. 