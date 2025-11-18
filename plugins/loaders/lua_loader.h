#pragma once 
#include "../plugin_manifest.h"

#include "../base/plugin_interface.h"
using namespace std; 

#include <string> 
#include <iostream>

namespace ibcs :: plugin :: lua_loader
{
    struct LuaPluginShim
    {
        PluginPtr instance = nullptr; 
        void *lua_state = nullptr;
    }; 

    LuaPluginShim load_lua_plugin(const PluginManifest &m, const string &script_path); 
    bool unload_lua_plugin(LuaPluginShim &r); 
} // namespace ibcs :: plugin :: lua_loader...// 

