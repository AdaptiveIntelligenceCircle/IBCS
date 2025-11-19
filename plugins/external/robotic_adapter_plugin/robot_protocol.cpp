#include "robot_protocol.h"

#include <iostream> 
using namespace std; 

namespace ibcs :: plugin :: external :: robot
{
    class DummyRobotProtocol : public IRobotProtocol
    {
        public: 
        bool connect(const string &endpoint) override 
        {
            cout <<"Robot proto" << "connect to " << endpoint << endl;
            connected = true;  
            return true; 
        }

        void disconnect() override 
        {
            cout <<"Robot proto" << "disconnect" << endl; 
            connected = false; 
        }

        bool send_motion(const MotionCommand &cmd) override 
        {
            if (!connected)
            return false; 

            cout <<"Robot proto - send motion: joints ="
            << cmd.joint_positions.size() << "duration = "
            << cmd.duration_seconds << "seconds" << endl; 
            return true; 
        }

        optional<RobotState> read_state() override 
        {
            if (!connected)
            return nullopt; 

            RobotState state; 
            state.battery = 95.0; 
            state.joint_positions = {0.0, 1.0, 0.5};
            state.joint_velocities = {0.0, 0.0, 0.0};
            state.estop_triggered = false; 
            state.raw_status = "OK";

            return state; 
        }

        bool ping() override 
        {
            return connected;
        }

        private: 
        bool connected = false ; 
    }; 

    IRobotProtocol *make_default_robot_protocol(){
        return new DummyRobotProtocol(); 
    }
} // namespace ibcs :: plugin :: external :: robot