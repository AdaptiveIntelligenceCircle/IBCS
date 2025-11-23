#pragma once 

#include <optional> 
#include <string> 

#include <array> 
using namespace std; 

namespace optimus :: hardware
{
    struct ImuSample
    {
        double timestamp = 0.0; 
        array<double, 3> accel = {0.0, 0.0, 0.0};
        array<double, 3>gyro = {0.0, 0.0, 0.0}; 
        array<double, 3>msg = {0.0, 0.0, 0.0}; 
        
        bool valid = true; 
    }; 

    class IIMuDriver
    {
        public: 
        virtual ~IIMuDriver () = default; 
        virtual bool connect(const string &endpoint) = 0; 
        virtual void disconnect() = 0; 
        
        // read the lastest sample (non-blocking).. 
        virtual optional<ImuSample> read_sample() = 0 ; 

        // Calibrate sensors (blocking).. 
        virtual bool calibrate(int duration_seconds = 3) = 0 ;
    };

    // Factory.. 
    IIMuDriver *make_stub_imu_driver(); 
}