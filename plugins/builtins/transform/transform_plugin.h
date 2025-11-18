#pragma once 
#include <string> 

#include <vector> 
#include <optional> 

using namespace std; 
#include "base/plugin_interface.h"

namespace ibcs :: plugin :: builtins
{
    struct TransformRule
    {
        string name; 
        string pattern; 
        string replace; 
    }; 

    class TransformPlugin : public IPlugin
    {
        public: 
        TransformPlugin(); 
        ~TransformPlugin() override;
        
        // IPlugin interface.. //
        PluginInfo info() const override;
        bool on_load() override;
        bool on_unload() override;
        vector<string> capabilities() const override;

        // Core functionality
        void add_rule(const TransformRule &rule);
        vector<TransformRule> list_rules() const;

        // API for the compiler to query mapping
        optional<string> apply(const string &input) const;

        private: 
        vector<TransformRule> rules_; 
    }; 
}

extern "C"
{
    ibcs :: plugin :: IPlugin* create_plugin(); 
    void destroy_plugin(ibcs :: plugin :: IPlugin*p); 
}