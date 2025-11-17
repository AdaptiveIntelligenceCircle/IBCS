#pragma once 
#include <string> 

#include <vector> 
using namespace std; 

namespace ibcs :: plugin
{
    struct PluginManifest
    {
        string name; 
        string version; 
        string entry; 

        string type; 
        vector<string> permissions; 
        vector<string> capabilities; 
    }; 

    bool load_manifest_from_json(const string &json_str, PluginManifest &out); 
}