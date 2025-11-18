#pragma once 
#include "../base/plugin_interface.h"

#include <string> 
#include <vector>

#include <fstream> 
using namespace std; 

namespace ibcs :: plugin :: builtins
{
    class LoggerPlugin : public IPlugin
    {
        public: 
        LoggerPlugin(); 
        ~LoggerPlugin() override;
        
        
        PluginInfo info() const override; 
        bool on_load() override ; 
        
        bool on_unload() override; 
        vector<string> capabilities() const override; 

        // logging API exposed to core or introspection
        void log(const string &level, const string &msg); 

        private: 
        ofstream logFile_; 
    }; 
}

extern "C"
{
    ibcs :: plugin :: IPlugin*create_plugin(); 
    void destroy_plugin(ibcs :: plugin :: IPlugin*p); 
}