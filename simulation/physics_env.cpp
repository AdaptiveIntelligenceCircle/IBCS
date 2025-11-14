#include <iostream> 
using namespace std ; 

#include "physics_env.h"
using namespace simulation; 

PhysicsEnv :: PhysicsEnv() : initialized_(false)
{}

PhysicsEnv :: ~PhysicsEnv()
{
    cout <<"[PhysicsEnv] Shutdown enviroment" << endl; 
}

void PhysicsEnv :: initialize()
{
    initialized_ = true; 
    world_state_ = {0.0f, 0.0f, 0.0f}; 
    cout <<"[PhysicsEnv] Initialized:" << endl; 
}

void PhysicsEnv :: step()
{
    if (!initialized_){
        std::cerr<<"[Physics ENv] Warning: not initialized" << endl; 
        return; 
    }
    // update dummy world state ..// 
    
    for (auto &val : world_state_)
    {
        val += 0.1f; 
    }

    cout <<"[Physics Env] World step : State[0] = " << world_state_[0] << endl; 
}