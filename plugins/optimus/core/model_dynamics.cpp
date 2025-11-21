#include "model_dynamics.h"
#include <algorithm>

namespace optimus::core {

bool ModelDynamics::init() { return true; }

std::vector<double> ModelDynamics::compute_torques(const StateManager &state, const WalkGait &gait) {
    std::vector<double> t(state.joint_count(), 0.0);
    for (size_t i = 0; i < t.size(); i++) {
        double err = gait.desired_positions[i] - state.joints()[i];
        t[i] = err * 10.0 - state.velocities()[i] * 0.5;
    }
    return t;
}

std::vector<double> ModelDynamics::compute_pose_torques(const StateManager &state, const PoseTrajectory &traj) {
    std::vector<double> t(state.joint_count(), 0.0);
    for (size_t i = 0; i < t.size(); i++) {
        double err = traj.target_positions[i] - state.joints()[i];
        t[i] = err * 8.0 - state.velocities()[i] * 0.3;
    }
    return t;
}

} // namespace optimus::core
