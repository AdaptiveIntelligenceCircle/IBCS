#pragma once 

#include <string>
#include <functional> 

#include <optional>
#include <vector> 

using namespace std; 

namespace optimus :: brainlink
{
    // High - level command coming from IBCS/Behaviorer.. 
    struct HLCommand
    {
        string id; 
        string action; 
        double velocity = 0.0; 
        double turn = 0.0; 

        vector<double> joints; 
        string object_label; 

        // Free-form metadata.. 
        string meta_json; 
    }; 

    // Telemetry pushed to IBCS .. 
    struct Telemetry
    {
        string id; // message id .. 
        double timestamp = 0.0; 

        vector<double> joints; 
        vector<double> velocities;
        
        double battery = 100.0; 
        bool safety_blocked = false; 
        bool fault = false; 
        string meta_json; // freeform.. 
    }; 

    using CommandCallback = function<void(const HLCommand*)>; 
    using CommandCallback = function<void(const Telemetry&)>;
    
    // Main interface between Optimus and Brain (IBCS).. // 
    class IBrainLink
    {
        public: 
        virtual ~IBrainLink() = default; 

        // connect to runtime(transport layer), returns true if success.. 
        virtual bool connect(const string &uri) = 0; 

        // publish telemetry upward to IBCS.. 
        virtual bool publish_telemetry(const Telemetry &t) = 0; 

        // send high-level command to Optimus (from IBCS).. 
        virtual bool send_command(const HLCommand &c) = 0; 

        // register callback to receive commands from IBCS (incoming).. 
        virtual void on_command(CommandCallback cb) = 0; 

        // Register callback to observe telemetry acknowledgement or responses...
        virtual void on_telemetry_ack(Telemetry cb) = 0; 

        // health check 
        virtual bool health_check() = 0; 
    }; 
} // namespace optimus:: brainlink.. 