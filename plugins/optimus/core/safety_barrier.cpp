#include "safety_barrier.h"
#include <algorithm>
#include <iostream>

namespace optimus::core 
{

bool SafetyBarrier::init() 
{
    blocked_ = false;
    fault_ = false;
    return true;
}

bool SafetyBarrier::check(const StateManager &state) 
{
    blocked_ = false;
    fault_ = false;

    // Battery check
    if (state.battery() < 1.0) 
    {
        cerr << "[Safety] Battery critically low!\n";
        blocked_ = true;
        return false;
    }

    // Joint limits (example +-3 rad)
    for (double j : state.joints()) 
    {
        if (j < -3.0 || j > 3.0) 
        {
            cerr << "[Safety] Joint limit exceeded\n";
            fault_ = true;
            blocked_ = true;
            return false;
        }
    }

    return true;
}

void SafetyBarrier::clamp_torque(vector<double> &torque) 
{
    for (auto &t : torque) 
    {
        t = std::clamp(t, -5.0, 5.0);
    }
}

} // namespace optimus::core
