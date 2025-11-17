#include "plugin_registry.h"
#include <mutex> 

using namespace std; 

namespace ibcs :: plugin
{
    PluginRegistry &PluginRegistry :: instance()
    {
        static PluginRegistry reg ;
        return reg; 
    }

    bool PluginRegistry:: register_plugin(const string &id, LoadedPlugin &plugin)
    {
        if (map_.count(id)) return false; 
        map_.emplace(id, move(plugin)); 
        return true; 
    }

    bool PluginRegistry :: unregister_plugin(const string &id)
    {
        return map_.erase(id) > 0; 
    }

    bool PluginRegistry :: has(const string &id) const
    {
        return map_.count(id) >  0; 
    }

    LoadedPlugin *PluginRegistry :: get(const string &id)
    {
        auto it = map_.find(id);
        if (it == map_.end()) return nullptr;
        return &it -> second;  
    }
} // namespace ibcs :: plugin