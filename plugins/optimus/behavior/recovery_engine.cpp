#include "recovery_engine.h"
using namespace std; 

#include <cmath> 
#include <iostream> 

namespace optimus :: behavior
{
    RecoveryEngine :: RecoveryEngine() = default; 
    RecoveryEngine :: ~RecoveryEngine() = default; 

    bool RecoveryEngine :: init()
    {
        return true; 
    }

    bool detect_fall(const vector<double> &joints, const vector<double> &velocities )
    {
        if (!joints.empty()){
            double torso = joints[0]; 
            if (abs(torso) > 1.2) return true; 
        }
        for (double v : velocities){
            if (abs(v)> 5.0) return true; 
        }
        return false; 
    }

    vector<vector<double>> RecoveryEngine :: stand_up_sequence(const vector<double> &current_state)
    {
        size_t n = max<size_t> (current_state.size(), 12); 
        vector<double> upbright(n, 0.0); 
        vector<vector<double>> seq; 

        int steps = 5; 

        for (int s = 0 ; s <steps ; ++s)
        {
            double alpha = double (s+ 1) / double(steps);
            vector<double> frame; 
            frame.reserve(n); 
            for(size_t i =0 ; i < n ; ++i)
            {
                double cur = (i< current_state.size())? current_state[i] : 0.0;
                frame.push_back(cur *(1.0 - alpha) + upbright[i] *alpha); 
            }
            seq.push_back(frame); 
        }
        return seq; 
    }
} // namespace optimus ;: behavior 