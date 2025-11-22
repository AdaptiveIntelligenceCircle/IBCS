#include "balance_engine.h"
#include <algorithm>

#include <cmath> 
using namespace std; 

namespace optimus :: behavior
{
    BalanceEngine :: BalanceEngine() = default; 
    BalanceEngine :: ~BalanceEngine() = default; 

    bool BalanceEngine :: init()
    {
        // nothing heavy.. 
        return true; 
    }

    void BalanceEngine :: set_gain(double kp, double kd)
    {
        kp_ = kp; 
        kd_ = kd; 
    }

    vector<double> BalanceEngine :: stabilize(const vector<double> &current_pos,
                                             const vector<double> &current_vel,
                                             const vector<double> &desired_pos,
                                             double dt)
    {
        size_t n = max({current_pos.size(), current_vel.size(), desired_pos.size() }); 
        vector<double> torques(n, 0.0); 
        for (size_t  i = 0 ; i < n ; ++i)
        {
            double cp = (i < current_pos.size())? current_pos[i] : 0.0; 
            double cv = (i < current_vel.size()) ? current_vel[i] : 0.0;
            double dp = (i < desired_pos.size()) ? desired_pos[i] : 0.0;

            double err = dp - cp; 
            double derr = -cv; 

            double t = kp_ *err + kd_*derr; 
            // safety clamp per joint to reasonable .. 
            t = clamp(t, -10.0, 10.0); 
            torques[i] = t; 
        }
        return torques; 
    }

    double BalanceEngine :: com_correction(double com_error)
    {
        // proportional only.. 
        double corr = kp_ *com_error; 
        if (corr > 0.5) corr = 0.5;
        if (corr < -0.5) corr = -0.5; 
        return corr; 
    }
} // namespace optimus :: behavior.. 