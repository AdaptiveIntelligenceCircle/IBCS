#pragma once 
#include <vector> 

using namespace std; 

namespace optimus :: behavior
{
    class RecoveryEngine
    {
        public: 
        RecoveryEngine(); 
        ~RecoveryEngine(); 

        bool init(); 

        // detect fall condition from joint positions/velocities (simple).. 
        bool detect_fall(const vector<double> &joints, const vector<double> &velocities ); 

        // Produce  a standup  sequence of joint targets
        vector<vector<double>> stand_up_sequence(const vector<double> &current_state); 
    }; 
}