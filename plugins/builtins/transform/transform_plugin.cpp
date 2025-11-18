// ibcs/plugins/builtins/transform/transform_plugin.cpp
#include "transform_plugin.h"
#include <iostream>

namespace ibcs::plugin::builtins {

TransformPlugin::TransformPlugin() {
}

TransformPlugin::~TransformPlugin() {
}

PluginInfo TransformPlugin::info() const {
    return {
        "builtins.transform",
        "1.0.0",
        "IBCS Core Team"
    };
}

bool TransformPlugin::on_load() {
    std::cout << "[transform] plugin loaded\n";

    // Default built-in rules
    rules_.push_back({
        "normalize-whitespace",
        R"(\s+)",
        " "
    });

    rules_.push_back({
        "inline-true",
        "true == true",
        "true"
    });

    return true;
}

bool TransformPlugin::on_unload() {
    std::cout << "[transform] plugin unloaded\n";
    rules_.clear();
    return true;
}

std::vector<std::string> TransformPlugin::capabilities() const {
    return {
        "transform",
        "rewrite_ast",
        "normalize"
    };
}

void TransformPlugin::add_rule(const TransformRule &rule) {
    rules_.push_back(rule);
}

std::vector<TransformRule> TransformPlugin::list_rules() const {
    return rules_;
}

std::optional<std::string> TransformPlugin::apply(const std::string &input) const {
    // Very simple implementation:
    // If 'pattern' is found in input, replace with 'replace'
    // This is a stub — real implementation uses regex or AST rewriting.

    for (const auto &r : rules_) {
        if (input.find(r.pattern) != std::string::npos) {
            std::string output = input;
            size_t pos = 0;
            while ((pos = output.find(r.pattern, pos)) != std::string::npos) {
                output.replace(pos, r.pattern.size(), r.replace);
                pos += r.replace.size();
            }
            return output;
        }
    }
    return std::nullopt;
}

} // namespace ibcs::plugin::builtins


// ======================= Loader Entry Points =======================

// extern "C" ibcs::plugin::IPlugin * create_plugin() {
//     return new ibcs::plugin::builtins::TransformPlugin();
// }

// extern "C" void destroy_plugin(ibcs::plugin::IPlugin* p) {
//     delete p;
// }
