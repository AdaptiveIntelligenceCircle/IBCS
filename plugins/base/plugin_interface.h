#pragma once 
#include <string> 
#include <vector> 
using namespace std; 

namespace ibcs :: plugin
{
    struct PluginInfo
    {
        string name; 
        string version; 
        string author; 
    }; 

    class IPlugin
    {
        public: 
        virtual ~IPlugin() = default;

        virtual PluginInfo info() const = 0 ; 

        virtual bool on_load() = 0; 
        virtual bool on_unload() = 0;
        
        virtual vector<string> capabilities() const = 0; 
    }; 
    using PluginPtr = IPlugin*; 
} // namespace ibcs :: plugin