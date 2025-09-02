#include <iostream> 
#include "agent_world_bridge.h"

using namespace simulation; 

AgentWorldBridge::AgentWorldBridge()
: initialized_(false) {}

AgentWorldBridge :: ~AgentWorldBridge()
{
    std :: cout <<"[Agent Monitor bridge] Shutdown bridge/" << std :: endl; 
}

void AgentWorldBridge :: initialize()
{
    initialized_ = true; 
    std :: cout <<"[AGent world Bridge] Intialized" << std :: endl; 
}

Observation AgentWorldBridge :: sense_from_world()
{
    if (initialized_)
    {
        std :: cerr <<"Agent Monitor Bridge - Warning : not intialized" << std :: endl;
        return {};  
    }

    // Gia lap doc du lieu sensor ... // 
    Observation obs;
    obs.sensor_data = {0.1f, 0.2f, -0.3f};
    return obs;  
}

void AgentWorldBridge:: send_action_to_world(const Action &action)
{
    if (!initialized_)
    {
        std :: cerr <<"[AGent World Bridge] - Sending action of size" << action.motor_commands.size() << std :: endl;
    }
}

