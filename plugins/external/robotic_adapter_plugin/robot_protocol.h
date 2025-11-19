#pragma once 
#include <string> 

#include <vector> 
#include <optional>

using namespace std; 

namespace ibcs :: plugin :: external :: robot
{
    // High level state from robot... 
    struct RobotState
    {
        double battery  = 100.0 ; 
        vector<double> joint_positions; 
        vector<double> joint_velocities; 
        bool estop_triggered = false; 
        string raw_status; 
    };
    
    // A generic motion command.. 
    struct MotionCommand
    {
        vector<double> joint_positions; 
        vector<double> joint_velocities; 

        double duration_seconds = 0.0; 
    }; 

    class IRobotProtocol
    {
        public: 
        virtual ~IRobotProtocol() = default; 

        // establish connection to robot endpoint.. 
        virtual bool connect(const string &endpoint) = 0; 

        // disconnect cleanly..
        virtual void disconnect() = 0; 

        // send a single motion command..
        virtual bool send_motion(const MotionCommand &cmd) = 0 ;
        
        // query low level state.. 
        virtual optional<RobotState> read_state() = 0; 

        // small health check. . 
        virtual bool ping() = 0; 
    };

    // factory for default protocol (dummy stab)
    IRobotProtocol *make_default_robot_protocol();
} // namespace ibcs :: plugin :: external :: robot