#pragma once 
#include <vector> 
#include <iostream>

namespace simulation
{
    class PhysicsEnv
    {
        public: 
        PhysicsEnv(); 
        ~PhysicsEnv(); 

        void initialize(); 
        void step(); 
        
        private:
        bool initialized_;
        std :: vector<float> world_state_; 
    }; 
}