#pragma once 
#include <string> 

#include <vector> 
#include <iostream>

using namespace std; 

#include "state_manager.h"
#include "model_kinematics.h"
namespace optimus :: core
{
    class ModelDynamics
    {
        public: 
        ModelDynamics() {};  
        ~ModelDynamics(){}; 

        bool init(); 
        vector<double> compute_torques(const StateManager &state, const WalkGait &gait); 
        vector<double> compute_pose_torques(const StateManager &state, const PoseTrajectory &traj); 
    }; 
} // namespace optimus :: core.. 