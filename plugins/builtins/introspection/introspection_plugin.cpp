#include "introspection_plugin.h"
using namespace std; 

#include <string> 
#include <iostream> 

#include "../../plugin_registry.h"

namespace ibcs :: plugin :: builtins
{
    IntrospectionPlugin :: IntrospectionPlugin()
    {
        // constructor .. 
    }

    IntrospectionPlugin :: ~IntrospectionPlugin()
    {
        // destructor .. // 
    }

    PluginInfo IntrospectionPlugin :: info() const 
    {
        return 
        {
            "builtins.introspection",
            "1.0.0",
            "IBCS Core Team"
        }; 
    }

    bool IntrospectionPlugin :: on_load()
    {
        cout <<"Introspection - Plugin loaded" << endl; 
        return true; 
    }

    bool IntrospectionPlugin :: on_unload()
    {
        cout <<"Introspection - Plugin unloaded" << endl; 
        return true; 
    }

    vector<string> IntrospectionPlugin :: capabilities() const 
    {
        return {
            "introspection",
            "dump_plugins", 
            "dump_memory"
        }; 
    }

    void IntrospectionPlugin :: dump_system_info()
    {
        cout << "------ Introspection: System Info----------" << endl; 
        cout <<"Architecture - IBCS plugin" << endl; 
        cout <<"Core version :" << endl; 
        cout << "-------------------------------------------" << endl; 
    }

    void IntrospectionPlugin :: dump_registered_plugins()
    {
        cout <<"------ Introspection : Registered plugins ---------" << endl; 

        cout <<"Registry listing extension needed" << endl;  
    }

    void IntrospectionPlugin :: dump_memory_state_stub()
    {
        cout << "Introspection Memory State" << endl; 
        cout <<"Memory snapshot not implemented" << endl; 
    }
}

extern "C" ibcs :: plugin :: IPlugin* create_plugin()
{
    return new ibcs :: plugin :: builtins :: IntrospectionPlugin(); 
}

extern "C" void destroy_plugin(ibcs :: plugin :: IPlugin *p)
{
    delete p; 
}
