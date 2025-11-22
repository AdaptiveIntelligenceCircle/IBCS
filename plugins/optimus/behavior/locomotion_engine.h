#pragma once 
#include <vector> 

#include <optional> 
using namespace std; 

#include "../core/model_kinematics.h"

namespace optimus :: behavior
{
    struct Step
    {
        double time; 
        vector<double> targets; 
        double support_phase; 
    }; 

    struct GaitPlan
    {
        double duration;
        vector<Step> steps; 
    }; 

    class LocomotionEngine
    {
        double duration; 
        vector<Step> steps;
    }; 

    class LocomotionEngine
    {
        public: 
        LocomotionEngine();
        ~LocomotionEngine(); 

        // intialize internal state(call once).. 
        bool init(); 

        GaitPlan plan_gait(double linear_velocity, double angular_rate, double duration = 100);
        
        // Generate conitnous target for time t in []
        // returns optional joint targets.. 
        optional <vector<double>> sample(const GaitPlan &plan, double t) const;
        
        // utilities . 
        void set_nominal_step_length(double meters); 
        void set_nominal_step_time(double seconds); 

        private: 
        double nominal_step_length_ = 0.25; 
        double nominal_step_time = 0.5; 
        // simple sinsoinal foot swing profile internal to engine.. 
    }; 
}