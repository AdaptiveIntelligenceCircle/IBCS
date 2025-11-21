#pragma once
#include <vector>
#include "state_manager.h"

namespace optimus::core {

class SafetyBarrier {
public:
    bool init();

    bool check(const StateManager &state);
    void clamp_torque(std::vector<double> &torque);

    bool is_blocked() const { return blocked_; }
    bool has_fault() const { return fault_; }

private:
    bool blocked_ = false;
    bool fault_ = false;
};

} // namespace optimus::core
