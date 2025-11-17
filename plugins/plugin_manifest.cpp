#include "plugin_manifest.h"
#include "../utils/mini_json.hpp"

#include <iostream> 
using namespace std; 

namespace ibcs :: plugin
{
    using namespace MiniJson; 

    bool load_manifest_from_json(const string &json_str, PluginManifest &out)
    {
        try 
        {
            auto v = parse_string(json_str);
            if (!v.is_object()) return false; 

            if (v.contains("name")) out.name = v["name"].as_string(); 
            if (v.contains("version")) out.name = v["version"].as_string(); 
            if (v.contains("entry")) out.name = v["entry"].as_string(); 
            if (v.contains("type")) out.name = v["type"].as_string();
            
            if (v.contains("Capabilities")){
                for (auto &c : v["Capabilities"].as_array())
                out.capabilities.push_back(c.as_string()); 
            }

            if (v.contains("Permission")){
                for (auto &c : v["Permission"].as_array())
                out.permissions.push_back(c.as_string()); 
            }
            return true; 
        }
        catch(const exception &ex)
        {
            cerr <<"PluginManifest - parse error:" << ex.what() << endl;
            return false; 
        }
    }
} // namespace ibcs :: plugin