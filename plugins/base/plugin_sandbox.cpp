#include "plugin_sandbox.h"
using namespace std; 

namespace ibcs :: plugin
{
    PluginSandbox :: PluginSandbox(SandboxPolicy p) : policy_(move(p)){}
    PluginSandbox :: ~PluginSandbox() = default; 

    bool PluginSandbox :: allows(const string &permission) const{
        for (auto &p : policy_.allowed_permissions)
        if (p == permission) return true; 
        return false; 
    }

    const SandboxPolicy &PluginSandbox :: policy() const
    {
        return policy_; 
    }

    bool PluginSandbox :: charge_cpu_time(chrono :: milliseconds ms)
    {
        cpu_used_ += ms; 
        return cpu_used_ <= policy_.max_cpu_time; 
    }

    void PluginSandbox :: reset_counters()
    {
        cpu_used_ = chrono :: milliseconds(0);
    }
} // namespace ibcs :: plugin.. 