#pragma once 
#include <string> 
#include <vector> 

#include <optional> 
#include <cstdint>
using namespace std; 

namespace optimus :: hardware
{
    struct JoinState
    {
        int id = -1;
        double position = 0.0; 
        double velocity = 0.0; 
        double torque = 0.0; 
    }; 

    struct MotorSpec
    {
        int id = -1; 
        double max_torque = 10.0; 
        double max_velocity = 10.0; 

        double position_lower = -3.14; 
        double position_upper = 3.14; 
        string model; 
    }; 

    class IMotorDriver
    {
        public: 
        virtual ~IMotorDriver() = default; 

        // connect to hardware endpoint (serial/CAN/IP), returns true on success.. 
        virtual bool connect(const string &endpoint) = 0; 

        // Disconnect gracefullt
        virtual void disconnect() = 0; 

        // Set target positions for a group of joints. returns true if needed.. 
        virtual bool set_positions(const vector<int> &joint_ids, const vector<double>&position); 

        // Query joint state for a specific id .. 
        virtual optional<JoinState> read_joint(int joint_id) = 0; 

        // Bulk read all known joints.. 
        virtual vector<JoinState> read_all() = 0 ; 

        // Get Motor specs.. 
        virtual vector<MotorSpec> specs() const = 0; 

        virtual bool set_torques(const vector<int> &joint_ads, const vector<double> &torques); 
        
    }; 

    // Factory : default stub driver.. 
    IMotorDriver *make_stub_mototr_driver(); 
} // namespace optimus :: hardware.. 