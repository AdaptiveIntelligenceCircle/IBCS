#include "manipulation_engine.h"
using namespace std; 

#include <iostream>
#include <algorithm>

#include <cmath>
#include <string> 

namespace optimus :: behavior
{
    ManipulationEngine :: ManipulationEngine() = default; 
    ManipulationEngine :: ~ManipulationEngine() = default; 

    bool ManipulationEngine :: init()
    {
        return true; 
    }

    optional<Grasp> ManipulationEngine :: plan_grasp(const string &object_label)
    {
        Grasp g; 
        if (object_label == "cube")
        {
            g.finger_positions = {0.6, 0.6, 0.6}; 
            g.quality = 0.9; 
        }else if(object_label == "Cyclinder"){
            g.finger_positions = {0.5, 0.5, 0.4}; 
            g.quality = 0.8; 
        }
        else 
        {
            // fallback generic pinch .. 
            g.finger_positions = {0.4, 0.4, 0.6}; 
            g.quality = 0.6; 
        }
        return g ; 
    }

    vector<vector<double>> ManipulationEngine ::plan_joint_trajectory(
    const vector<double> &current,
    const vector<double> &target,
    double duration,
    int steps 
    )
    {
        vector<vector<double>> traj; 
        if (steps <= 0) return traj; 

        for (int s = 0; s < steps ; ++s)
        {
            double alpha = double(s)/ double(max(1, steps -1)); 
            // traj.push_back(lerp(current, target, alpha)); 
        }
        return traj; 
    }

//     vector<double> ManipulationEngine::lerp(const vector<double> &a, const vector<double> &b, double alpha) 
//     {
//     size_t n = max(a.size(), b.size());
//     vector<double> out(n, 0.0);
//     for (size_t i=0;i<n;++i) 
//     {
//         double va = (i < a.size()) ? a[i] : 0.0;
//         double vb = (i < b.size()) ? b[i] : 0.0;
//         out[i] = va*(1.0-alpha) + vb*alpha;
//     }
//     return out;
// }
} // namespace optimus :: behavior.. 