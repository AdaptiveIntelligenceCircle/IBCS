#include "power_system.h"
#include <iostream> 

#include <chrono> 
#include <thread> 

#include <atomic> 
using namespace std; 

namespace optimus :: hardware
{
    class StubPowerSystem : public IPowerSystem
    {
        public: 
        StubPowerSystem()
        {
            status_.battery_pct = 100.0;
            status_.voltage = 48.0;
            status_.current = 0.0;
            status_.charging = false;
            status_.critical = false;
            running_ = false;
        }

        ~StubPowerSystem()
        {
            running_ = false; 
            if (worker_.joinable()) worker_.join(); 
        }

        bool init(const string &cfg_endpoint = "") override
        {
            cfg_ = cfg_endpoint; 
            running_ = true; 
            worker_ = thread([this]()
            {
                this_thread :: sleep_for(chrono:: seconds(1)); 
                // drain slowly 
                status_.battery_pct -= 0.01; 
                if (status_.battery_pct < 20.0) 
                status_.critical = true; 
                status_.voltage = 48.0 *(status_.battery_pct/100.0); 
                status_.current = 2.0; 
            }); 

            cout << "Power Stub init" << cfg_endpoint << endl; 
            return true; 
        }

        PowerStatus status() override
        {
            return status_; 
        }

        bool set_power_mode(const string &mode) override
        {
            if (mode == "low_power"){
                status_.current = 0.5; 
                cout <<"Power stub  - set low power" << endl; 
                return true; 
            }
            else if (mode == "normal"){
                status_.current = 2.0; 
                cout <<"Power stub set normal" << endl; 
                return true; 
            }
            else if(mode == "shutdown"){
                emergency_shutdown(); 
                return true; 
            }
            return false; 
        }

        bool emergency_shutdown() override{
            cout <<"Power stub- emergency shutdown intiated" << endl;
            // simulate immediate stop.. 
            status_.battery_pct = 0.0; 
            status_.critical = true;
            return true; 
        }
        
        private: 
        string cfg_; 
        PowerStatus status_; 
        atomic<bool> running_; 
        thread worker_; 
    }; 

    IPowerSystem *make_stub_power_system()
    {
        return; 
    }
}