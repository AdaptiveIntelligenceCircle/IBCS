#include "locomotion_engine.h"
#include <cmath> 

#include <iostream> 
using namespace std; 

namespace optimus :: behavior
{
    LocomotionEngine :: LocomotionEngine() = default; 
    LocomotionEngine :: ~LocomotionEngine() = default; 

    bool LocomotionEngine :: init()
    {
        // nothing heavy for now.. 
        return true; 
    }

    void LocomotionEngine :: set_nominal_step_length(double meters)
    {
        nominal_step_length_ = meters; 
    }

    void LocomotionEngine :: set_nominal_step_time(double seconds)
    {
        nominal_step_time = seconds;
    }

    GaitPlan LocomotionEngine :: plan_gait(double linear_velocity,double angular_rate, double duration)
    {
        GaitPlan plan; 
        plan.duration = duration; 

        double step_time = nominal_step_time; 
        if(abs(linear_velocity) > 1e-3){
            // faster velocit => slightly shorter step time.. 
            step_time = max(0.2, nominal_step_time* max(0.5,0.5 + 0.5 * (0.5 - std::abs(linear_velocity))));
        }

        int nsteps = max(1, int(ceil(duration/step_time))); 

        double t = 0.0; 

        for (int i = 0 ; i < nsteps ; ++i)
        {
            Step s; 
            s.time = t; 

            s.targets.assign(12, 0.0); 
            double frontend = linear_velocity *step_time; 
            double yaw = angular_rate *step_time *(( i % 2 == 0) ? 1.0 : -1.0); 
            for (size_t j = 0; j <s.targets.size(); ++j)
            {
                // encode rough gait : alterning legs(even indices).. 
                double side = ((j % 2) == 0) ? 1.0 : -1.0; 
                s.targets[i] = side *0.05 *sin((double) i *0.5); 
            }

            s.support_phase = ((i % 2) == 0) ? 0.2 : 0.8;
            plan.steps.push_back(move(s)); 
            t += step_time;  

        }
        return plan; 
    }

    optional<vector<double>> LocomotionEngine :: sample(const GaitPlan &plan, double t) const
    {
        if (plan.steps.empty()) return nullopt; 
        if (t <= 0.0) return plan.steps.front().targets;    
        if (t >= plan.duration) return plan.steps.back().targets; 

        // find two surrounding steps and literaly interpolate.. 
        int idx = 0; 
        while (idx + 1 < (int)plan.steps.size() and plan.steps[idx + 1].time); 
        const Step &a = plan.steps[idx]; 
        const Step &b = (idx + 1 < (int) plan.steps.size() ? plan.steps[idx + 1] : a);
        
        double local_dt = (b.time - a.time); 
        double alpha = (local_dt <= 1e-9) ? 0.0 : ((t - a.time)/ local_dt); 

        vector<double> out; 
        out.resize(a.targets.size()); 
        for (size_t i = 0; i < out.size(); ++i)
        out[i] = a.targets[i] *(1.0 - alpha) + b.targets[i] *alpha; 
        return out; 
    }
}