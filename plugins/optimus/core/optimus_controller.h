#pragma once 
#include <vector> 

#include <string> 
#include <memory> 

#include <cstdint> 
using namespace std; 

#include "state_manager.h"
#include "safety_barrier.h"

#include "model_dynamics.h"
#include "model_kinematics.h"

namespace optimus :: core
{
    struct HighLevelCommand
    {
        enum class Type{
            NONE, WALK, STOP, POSE, MANIPULATE
        }type = Type :: NONE; 

        // pose target ..
        vector<double> target_joint_positions;
        double walk_velocity = 0.0; 
        double turn_rate = 0.0; 

        // Manipulation parameters . 
        string object_label; 
    }; 

    struct RobotTelementary
    {
        vector<double> joints;
        vector<double> velocities;
        bool safety_blocked = false; 

        bool fault = false; 
        double battery  = 100.0; 
    }; 

    class OptimusController
    {
        public: 
        OptimusController(); 
        ~OptimusController(); 

        bool init(); 
        bool update(double dt); 
        bool apply_command(const HighLevelCommand &cmd);
        RobotTelementary get_telementry() const; 

        private: 
        StateManager state_; 
        SafetyBarrier safety_;
        ModelKinematics kinematics_;
        ModelDynamics dynamics_; 

        // Cached high level command .. 
        HighLevelCommand current_cmd;
    }; 
}
