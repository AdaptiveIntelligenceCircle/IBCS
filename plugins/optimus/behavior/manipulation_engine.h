#pragma once 
#include <vector> 
#include <string> 

#include <algorithm>
using namespace std; 

namespace optimus :: behavior
{
    struct Grasp
    {
        vector<double> finger_positions; 
        double quality = 0.0; 
    }; 

    class ManipulationEngine
    {
        public: 
        ManipulationEngine(); 
        ~ManipulationEngine(); 

        bool init(); 

        // plan grasp for object (stub returns pre - backed grasp..)
        optional<Grasp> plan_grasp(const string &object_label); 

        // Generate joint trajectory to reach target joint posture.. 
        vector<vector<double>> plan_joint_trajectory(
            const vector<double> &current,
            const vector<double> &target,
            double duration,
            int steps
        );

        // simple path somehing utility.. 
        static vector<double> lerp(const vector<double> &a,vector<double> &b, double alpha); 
        
    }; 
}