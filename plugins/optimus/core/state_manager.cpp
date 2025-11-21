#include "state_manager.h"
using namespace std; 

#include <algorithm>
#include <cmath> 

namespace optimus :: core
{
    StateManager :: StateManager(){}
    StateManager :: ~StateManager(){}

    bool StateManager :: init()
    {
        joints_.assign(12, 0.0); 
        velocities.assign(12, 0.0); 
        battery = 100.0; 

        return true;  
    }

    void StateManager :: update(double dt)
    {
        // Simulate battery drain.. // 
        battery = dt * 0.01; 
        if (battery < 0.0) battery = 0.0; 

        // simple physics intergrations.. 
        for (size_t i = 0 ; i < joints_.size() ; i++)
        {
            joints_[i] += velocities[i] *dt; 
        }
    }

    void StateManager :: apply_torque(const vector<double> &torque, double dt)
    {
        for (size_t i = 0; i < velocities.size(); i++)
        {
            velocities[i] += torque[i] * dt; 
            // mass simplified.. 
        }
    }

    const vector<double> &StateManager :: joints() const 
    {
        return joints_ ; 
    }

    // const vector<double> &StateManager :: velocities() const 
    // {
    //     return velocities; 
    // }

    size_t StateManager :: joint_count() const 
    {
        return joints_.size(); 
    }

    // double StateManager :: battery() const 
    // {
    //     return battery; 
    // }

}
