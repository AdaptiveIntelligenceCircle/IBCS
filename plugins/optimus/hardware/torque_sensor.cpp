#include "torque_sensor.h"
#include <iostream>

#include <mutex> 
#include <unordered_map>

using namespace std; 

namespace optimus :: hardware
{
    class StubTorqueSensor : public ITorqueSensor
    {
        public: 
        StubTorqueSensor(){
            // init default zero torques.. 
            for (int i = 0 ; i < 12; i++)
            {
                TorqueReading r; 
                r.joint_id = i; 
                r.torque = 0.0; 
                r.timestamp = 0.0 ; 
                readings_[i] = r; 
            }
        }
        ~StubTorqueSensor() override = default; 

        bool connect(const string &endpoint) override 
        {
            endpoint_ = endpoint; 

            cout <<"Torque Sensor Stub - connected to " << endpoint << endl; 
            return true; 
        }

        void disconnect() override 
        {
            cout <<"Torque SensorStub" << "disconnected" << endl; 
        }

        optional<TorqueReading> read(int joint_id) override 
        {
            lock_guard<mutex> lk(mu_); 
            auto it = readings_.find(joint_id); 
            if (it == readings_.end()) 
            return nullopt; 
            return it -> second; 
        }

        private: 
        string endpoint_ ; 
        string endpoint_; 
        mutex mu_; 

        unordered_map<int, TorqueReading> readings_; 
    }; 

    ITorqueSensor * make_stub_torque_sensor()
    {
        return; 
    }
}