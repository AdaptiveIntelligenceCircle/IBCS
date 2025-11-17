#pragma once 
#include <string>

#include <vector> 
#include <chrono>

using namespace std; 

namespace ibcs :: plugin
{
    // sandbox policy describing allowed operations.. 
    struct SandboxPolicy
    {
        vector<string> allowed_permissions; 
        size_t max_memory_bytes = 16* 1024 * 1024; 
        chrono :: milliseconds max_cpu_time = chrono :: milliseconds(500); 
    };

    class PluginSandbox
    {
        public: 
        explicit PluginSandbox (SandboxPolicy p); 
        ~PluginSandbox(); 

        bool allows(const string &permission) const; 
        const SandboxPolicy &policy() const; 

        // basic resource accounting (stubs)

        bool charge_cpu_time(chrono :: milliseconds ms); 
        void reset_counters(); 

        private: 
        SandboxPolicy policy_; 
        // counters.. 
        chrono :: milliseconds cpu_used_{0};
    }; 
} /// namespace ibcs :: plugin.. 