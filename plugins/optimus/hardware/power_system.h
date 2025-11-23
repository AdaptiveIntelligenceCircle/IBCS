#pragma once 
#include <optional> 

#include <string> 
using namespace std; 

namespace optimus :: hardware
{
    struct PowerStatus
    {
        double battery_pct = 100.0; 
        double voltage = 48.0; 
        double current = 0.0; 
        bool charging = false; 
        bool critical = false; 

        string raw_state; 
    }; 

    class IPowerSystem
    {
        public: 
        virtual ~IPowerSystem() = default; 
        virtual bool init(const string &cfg_endpoint = "") = 0; 
        virtual PowerStatus status() = 0; 
        virtual bool set_power_mode(const string &mode) =0; 
        virtual bool emergency_shutdown() = 0; 
    }; 
    // factory .. 
    IPowerSystem *make_stub_power_system(); 
} // namespace optimus :: hardware.. 