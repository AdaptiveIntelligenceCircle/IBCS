// ibcs/plugins/external/robotic_adapter_plugin/robotic_adapter_plugin.cpp
#include "robot_plugin.h"
#include <iostream>
#include <algorithm>

namespace ibcs::plugin::external::robot {

RoboticAdapterPlugin::RoboticAdapterPlugin()
    : proto_(make_default_robot_protocol())
{
}

RoboticAdapterPlugin::~RoboticAdapterPlugin() {
}

ibcs::plugin::PluginInfo RoboticAdapterPlugin::info() const {
    return {
        "external.robotic_adapter",
        "1.0.0",
        "IBCS External Robotics"
    };
}

bool RoboticAdapterPlugin::on_load() {
    std::cout << "[robotic_adapter] plugin loaded\n";
    return true;
}

bool RoboticAdapterPlugin::on_unload() {
    std::cout << "[robotic_adapter] plugin unloaded\n";
    if (proto_) proto_->disconnect();
    return true;
}

std::vector<std::string> RoboticAdapterPlugin::capabilities() const {
    return {
        "robot.send",
        "robot.read",
        "robot.motion_adapt",
        "safety.guard"
    };
}

bool RoboticAdapterPlugin::configure(const std::string &endpoint) {
    endpoint_ = endpoint;
    if (!proto_->connect(endpoint_)) {
        std::cerr << "[robotic_adapter] could not connect to " << endpoint << "\n";
        return false;
    }
    if (!proto_->ping()) {
        std::cerr << "[robotic_adapter] ping failed\n";
        return false;
    }
    std::cout << "[robotic_adapter] configured successfully\n";
    return true;
}

// safety: clamp ranges & duration
bool RoboticAdapterPlugin::safety_filter(MotionCommand &cmd) const {
    if (cmd.duration_seconds <= 0.0 || cmd.duration_seconds > 5.0) {
        std::cerr << "[robotic_adapter] duration out of bounds\n";
        return false;
    }

    if (!cmd.joint_positions.empty())
        clamp_vec(cmd.joint_positions, -2.0, 2.0); // rad or m
    if (!cmd.joint_velocities.empty())
        clamp_vec(cmd.joint_velocities, -1.0, 1.0);

    return true;
}

void RoboticAdapterPlugin::clamp_vec(std::vector<double> &v, double lo, double hi) const {
    for (auto &x : v) {
        if (x < lo) x = lo;
        if (x > hi) x = hi;
    }
}

bool RoboticAdapterPlugin::send_motion_command(MotionCommand cmd) {
    if (!proto_) return false;

    if (!safety_filter(cmd)) {
        std::cerr << "[robotic_adapter] safety_filter blocked motion\n";
        return false;
    }
    return proto_->send_motion(cmd);
}

optional<RobotState> RoboticAdapterPlugin::get_state() {
    if (!proto_) return nullopt;
    return proto_->read_state();
}

extern "C" ibcs::plugin::IPlugin* create_plugin() {
    return new RoboticAdapterPlugin();
}

extern "C" void destroy_plugin(ibcs::plugin::IPlugin* p) {
    delete p;
}

} // namespace ibcs::plugin::external::robot
