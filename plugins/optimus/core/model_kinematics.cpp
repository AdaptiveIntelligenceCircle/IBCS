#include "model_kinematics.h"
#include <iostream>

namespace optimus :: core
{
    bool ModelKinematics :: init()
    {
        return true; 
    }

    WalkGait ModelKinematics :: plan_walk(double velocity, double turn_rate)
    {
        WalkGait g; 
        g.desired_positions.assign(12, velocity * 0.01 + turn_rate * 0.005); 
        return g; 
    }

    PoseTrajectory ModelKinematics :: solve_it (const vector<double> &target){
        PoseTrajectory p; 

        p.target_positions = target;  
        return p; 
    }
    
    PoseTrajectory ModelKinematics :: manipulate(const string &object_label)
    {
        PoseTrajectory p; 

        // Example : each object label maps to default posture.. 
        if (object_label == "cube")
        {
            p.target_positions.assign(12, 0.3); 
        }else 
        {
            p.target_positions.assign(12, 0.1); 
        }
        return p; 
    }
} // namespace optimus :: core.. 