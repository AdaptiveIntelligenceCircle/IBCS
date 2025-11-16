#include "config_registry.h"

namespace ibcs::config 
{

RuntimeConfig ConfigRegistry::runtime_;
SecurityConfig ConfigRegistry::security_;
IntrospectionConfig ConfigRegistry::introspection_;

void ConfigRegistry::set(const RuntimeConfig &r) { runtime_ = r; }
void ConfigRegistry::set(const SecurityConfig &s) { security_ = s; }
void ConfigRegistry::set(const IntrospectionConfig &i) { introspection_ = i; }

const RuntimeConfig& ConfigRegistry::runtime() { return runtime_; }
const SecurityConfig& ConfigRegistry::security() { return security_; }
const IntrospectionConfig& ConfigRegistry::introspection() { return introspection_; }

} // namespace ibcs::config
