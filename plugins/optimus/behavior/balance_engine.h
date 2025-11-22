#pragma once 
#include <vector> 

using namespace std; 

namespace optimus :: behavior
{
    class BalanceEngine
    {
        public: 
        BalanceEngine(); 
        ~BalanceEngine(); 

        bool init(); 

        vector<double> stabilize(const vector<double> &current_pos, 
        const vector<double> &current_vel, const vector<double> &desired_pos, 
        double dt);
        
        // Compute center of mass effect correction.. 
        double com_correction(double com_error); 
        void set_gain(double kp, double dt); 
        
        private: 
        double kp_ = 30.0; 
        double kd_ = 2.0; 
        
    }; 
}