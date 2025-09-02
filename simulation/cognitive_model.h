#pragma once 
#include <iostream> 
#include <string> 
#include <cmath> 

#include "agent_world_bridge.h"

namespace simulation
{
    class CognitiveModel
    {
        public: 
        CognitiveModel(); 
        ~CognitiveModel(); 

        void initialize(); 
        Action decide(const Observation &obs); 

        private : 
        bool initialized_;
    }; 
}