#pragma once 
#include "../../base/plugin_interface.h"

#include <string> 
#include <vector>

using namespace std; 
#include <iostream> 

namespace ibcs :: plugin :: builtins
{
    class IntrospectionPlugin : public IPlugin
    {
        public: 
        IntrospectionPlugin(); 
        ~IntrospectionPlugin() override; 

        PluginInfo info() const override; 
        bool on_load() override; 
        bool on_unload() override ; 

        vector<string> capabilities() const override ; 

        // Builtin capabilities.. 
        void dump_system_info();
        void dump_registered_plugins();
        void dump_memory_state_stub();  
    };  
} // namespace ibcs :: plugin :: builtins.. 

extern "C"
{
    ibcs :: plugin :: IPlugin *create_plugin(); 
    void destroy_plugin(ibcs :: plugin :: IPlugin*p);
}
