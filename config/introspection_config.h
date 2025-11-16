#pragma once

namespace ibcs::config {

struct IntrospectionConfig {
    bool enable = true;
    bool expose_opcode_table = true;
    int max_depth = 10;
};

} // namespace ibcs::config
