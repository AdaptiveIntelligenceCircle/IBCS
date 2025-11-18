#pragma once 
#include <string> 

#include "../plugin_manifest.h"
#include "../base/plugin_interface.h"

#include <iostream>
using namespace std; 

namespace ibcs :: plugin :: native 
{
    struct NativeLoaderResult
    {
        void *handle = nullptr; 
        PluginPtr instance = nullptr; 
    }; 

    // loads a native shared library and instantiates plugin by conventions: 
    NativeLoaderResult load_native_plugin(const PluginManifest &m, const string &filepath); 
    bool unload_native_plugin(NativeLoaderResult &r); 
}