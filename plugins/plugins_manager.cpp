#include "plugins_manager.h"

#include "loaders/native_loader.h"
#include "loaders/lua_loader.h"

#include "plugin_manifest.h"
#include "plugin_registry.h"

#include "../utils/mini_json.hpp"
#include <fstream> 

#include <iostream> 
using namespace std; 

namespace ibcs :: plugin
{
    using namespace MiniJson; 

    PluginManager :: PluginManager(){} 
    PluginManager :: ~PluginManager() 
    {
        auto &reg = PluginRegistry ::instance(); 

        // copy keys to avoid mutation during iterations.. 
        vector<string> ids; 

        // for (auto &p :{}) {
        //     (void)p; 
        // }
    }

    SandboxPolicy PluginManager :: default_policy_for(const PluginManifest &m) const 
    {
        SandboxPolicy p; 
        // Conservative defaults ; allow introspection for builtin
        p.allowed_permissions = {}; 
        for (auto &perm : m.permissions) p.allowed_permissions.push_back(perm); 
        if(m.name == "builtins.introspection")
        p.allowed_permissions.push_back("introspection"); 
        return p; 
    }
    
    bool PluginManager :: load_plugin_from_file(const string &manifest_path)
    {
        ifstream in (manifest_path); 
        if (!in)
        {
            cout <<"Plugin Manager - Cannot open manifest:" << manifest_path << endl; 
            return false; 
        }

        stringstream buf; 
        buf << in.rdbuf(); 
        string txt  = buf.str(); 

        PluginManifest m  ; 
        if (!load_manifest_from_json(txt, m))
        {
            cout <<"PluginManager - manifest parse failed" << endl; 
            return false; 
        }

        if (m.type == "native") return load_native(m); 
        if (m.type == "lua") return load_lua(m); 
        if (m.type == "Wasm") return load_wasm(m); 

        cout <<"Plugin Manager - unknown plugin type :" << m.type << endl; 
        return false; 
    }

    bool PluginManager :: load_native(const PluginManifest &m)
    {
        auto res = native :: load_native_plugin(m, m.entry); 
        if (!res.instance)
        {
            cerr <<"PluginManager - native load failed for" << m.name << endl; 
            return false; 
        }

        // check capabilities and permissions (basic)...
        SandboxPolicy sp = default_policy_for(m); 
        auto sandbox = make_unique<PluginSandbox> (sp); 

        // call on load under sandbox guard...
        bool ok = res.instance -> on_load() ; 
        if (!ok)
        {
            native :: unload_native_plugin(res); 
            cout <<"PluginManager - Plugin on load rejected\n"; 
            return false; 
        }

        LoadedPlugin lp;
        lp.manifest = m; 
        // lp.sandbox = move(sandbox) ; 
        lp.instance = res.instance;
        lp.native_handler = res.handle;
        // lp.loaded_from = res.entry; 
        return true; 
    }
    bool PluginManager::load_wasm(const PluginManifest &m) 
    {
    // Not implemented: placeholder to integrate a WASM runtime (Wasmtime / Wasm3)
    cerr << "[PluginManager] WASM loader not implemented yet. Skipping plugin " << m.name << "\n";
    return false;
    }

    bool PluginManager::unload_plugin(const string &plugin_id) 
    {
    auto lp = PluginRegistry::instance().get(plugin_id);
    if (!lp) return false;

    // call on_unload
    if (lp->instance) lp->instance->on_unload();

    // if native handle: try destroy
    if (lp->native_handler) 
    {
        // attempt to destroy with native loader: assume symbol destroy_plugin exists
    #if defined(_WIN32)
        // Windows branch omitted for brevity
        (void)lp;
    #else
        // attempt to dlsym destroy_plugin
        using namespace native;
        NativeLoaderResult r;
        r.handle = lp->native_handle;
        r.instance = lp->instance;
        unload_native_plugin(r);
    #endif
    } 
    else 
    {
        // maybe lua: call unload via lua_loader (we cannot detect lua state here)
        // best-effort: nothing to do
    }

    PluginRegistry::instance().unregister_plugin(plugin_id);
    cout << "[PluginManager] unloaded plugin: " << plugin_id << "\n";
    return true;
    }

    vector<string> PluginManager :: list_plugin() const 
    {
        vector<string> out;

        return out; 
    }

    vector<string> PluginManager :: find_plugins_with_capabilities(const string &cap) const 
    {
        vector<string> out; 
        return out; 
    }

    unique_ptr<PluginManager> make_plugin_manager()
    {
        return make_unique<PluginManager>(); 
    }
}