#pragma once 
#include <vector> 

#include <cstdint> 
using namespace std; 

namespace optimus :: core  
{
    class StateManager 
    {
        public: 
        StateManager(); 
        ~StateManager(); 

        bool init(); 
        void update(double dt); 

        const vector<double> &joints() const; 
        const vector<double>& velocities() const; 
        size_t joint_count() const; 

        void apply_torque(const vector<double> &torque, double dt); 

        double battery() const; 

        private: 
        vector<double> joints_ ; 
        vector<double> velocities; 
        double battery = 100.0; 
    }; 
}