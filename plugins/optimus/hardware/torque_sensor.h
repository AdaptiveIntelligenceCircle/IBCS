#pragma once 
#include <optional> 
#include <iostream> 

#include <string> 
#include <vector> 
using namespace std; 

namespace optimus :: hardware
{
    // single joint torque reading.. 
    struct TorqueReading
    {
        int joint_id = -1; 
        double torque = 0.0; 
        double timestamp = 0.0; 
    }; 

    class ITorqueSensor
    {
        public: 
        virtual ~ITorqueSensor() = default; 
        virtual bool connect(const string &endpoint) = 0; 
        virtual void disconnect() = 0; 

        virtual optional<TorqueReading> read(int joint_id) = 0; 
        virtual vector<TorqueReading> read_all() = 0; 
    }; 

    ITorqueSensor *make_stub_torque_sensor(); 
}   // namespace optimus :: hardware.. 