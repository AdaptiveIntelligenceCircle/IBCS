#pragma once 
#include <vector>

#include <string> 
using namespace std; 

namespace optimus :: core
{
    struct WalkGait
    {
        vector<double> desired_positions; 
    }; 

    struct PoseTrajectory
    {
        vector<double> target_positions; 
    }; 

    class ModelKinematics
    {
        public: 
        bool init(); 

        WalkGait plan_walk(double velocity, double turn_rate); 
        PoseTrajectory solve_it (const vector<double> &target);
        PoseTrajectory manipulate(const string &object_label); 
    };
} // namespace optimus :: core.. 