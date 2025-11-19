#pragma once 
#include "../../base/plugin_interface.h"
#include "robot_protocol.h"

#include <memory> 
#include <string> 

#include <vector> 
#include <optional>

namespace ibcs :: plugin :: external :: robot 
{
    class RoboticAdapterPlugin : public ibcs :: plugin :: IPlugin
    {
        public: 
        RoboticAdapterPlugin();
        ~RoboticAdapterPlugin(); 

        // IPlugin 
        ibcs :: plugin :: PluginInfo info() const override; 
        bool on_load() override; 
        bool on_unload() override;
        
        vector<string> capabilities() const override; 

        // setup protocol and (endpoint = Ip/serial...)
        bool configure(const string &endpoint); 

        // control operations.. 
        bool send_motion_command(const MotionCommand cmd); 
        optional<RobotState> get_state();
        
        // health check..
        bool ping(); 

        private: 
        unique_ptr<IRobotProtocol> proto_;
        string endpoint_; 

        // small safety guard..
        bool safety_filter(MotionCommand &cmd) const ;
        
        // Saturation helpers.. 
        void clamp_vec(vector<double> &v, double lo, double hi) const; 
    }; 
    extern "C"
    {
    ibcs :: plugin :: IPlugin *create_plugin(); 
    void destroy_plugin(ibcs :: plugin :: IPlugin *p); 
    } 
} // namespace ibcs :: plugin :: external :: robot
