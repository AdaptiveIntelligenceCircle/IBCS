#pragma once 
#include <string> 
#include <vector> 
#include <iostream>

namespace simulation
{
    struct Observation
    {
        std :: vector<float> sensor_data; 
    }; 

    struct Action
    {
        std :: vector<float> motor_commands; 
    }; 

    class AgentWorldBridge
    {
        public: 
        AgentWorldBridge(); 
        ~AgentWorldBridge(); 

        void initialize(); 
        Observation sense_from_world(); 
        void send_action_to_world(const Action &action) ;

        private: 
        bool initialized_;
    };  
}   
