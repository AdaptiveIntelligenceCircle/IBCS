#include "optimus_controller.h"
using namespace std; 

#include <string> 
#include <vector>

#include <cmath> 
#include <iostream>

namespace optimus :: core 
{
    OptimusController :: OptimusController(){}
    OptimusController :: ~OptimusController(){}

    bool OptimusController ::init()
    {
        cout <<"Optimus Controller - init" << endl; 
        state_.init(); 
        safety_.init(); 
        kinematics_.init(); 
        dynamics_.init(); 
        
        return true; 
    }

    bool OptimusController :: apply_command(const HighLevelCommand &cmd)
    {
        current_cmd = cmd; 
        return true; 
    }

    bool OptimusController :: update(double dt)
    {
        state_.update(dt); 

        // 1. Safety check first.. 
        if (!safety_.update(dt) and !safety_.check(state_))
        {
            cout << "Safety Barrier triggerred" << endl; 
            return false; 
        }

        // 2. Apply high level commands. . 
        switch(current_cmd.type){
            case HighLevelCommand :: Type :: WALK  : 
            {

            auto gait = kinematics_.plan_walk(current_cmd.walk_velocity, current_cmd.turn_rate);
            auto torques = dynamics_.compute_torques(state_, gait); 
            safety_.clamp_torque(torques); 
            state_.apply_torque(torques, dt); 

            } 
            break;  
            case HighLevelCommand :: Type :: POSE : 
            {
                auto traj = kinematics_.solve_ik(current_cmd.target_joint_positions); 
                auto torques = dynamics_.compute_pose_torques(state_ , traj);
                safety_.clamp_torques(torques);
                safety_.apply_torques(torques, dt); 
            }
            break; 

            case HighLevelCommand::Type::MANIPULATE:
            {
            auto traj = kinematics_.manipulate(current_cmd.object_label);
            auto torques = dynamics_.compute_pose_torques(state_, traj);
            safety_.clamp_torque(torques);
            state_.apply_torque(torques, dt);
            }
            break;

            case HighLevelCommand::Type::STOP:
            // zero torques
            state_.apply_torque(std::vector<double>(state_.joint_count(), 0.0), dt);
            break;

            default:
            break;
        }
        return true;
    }

    RobotTelementary OptimusController :: get_telementry() const 
    {
        RobotTelementary t; 
        t.joints = state_.joints(); 
        t.velocities = state_.velocities(); 
        t.safety_blocked = state_.is_blocked(); 
        t.fault  = safety_.has_fault();
        t.battery = state_.battery();
        return t; 
    }
} // namespace optimus :: core.. 