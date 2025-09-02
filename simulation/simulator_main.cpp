#include <iostream> 
#include "cognitive_model.h"
#include "physics_env.h"
#include "agent_world_bridge.h"

#include "cognitive_model.cpp"
#include "physics_env.cpp"
#include "agent_world_bridge.cpp"

using namespace simulation;

AgentWorldBridge bridge;
CognitiveModel cog;
PhysicsEnv env;

// bridge.initialize();
// cog.initialize();
// env.initialize();

int main ()
{
    for (int i=0; i<10; i++) {
    auto obs = bridge.sense_from_world();
    auto act = cog.decide(obs);
    bridge.send_action_to_world(act);
    env.step();
    return 0; 
    }
    return 0; 
}
