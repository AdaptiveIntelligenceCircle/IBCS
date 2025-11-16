#pragma once 
#include "runtime_config.h"

#include "security_config.h"
#include "introspection_config.h"

#include "../utils/mini_json.hpp"
#include "../utils/mini_json.hpp"

#include <string>
using namespace std;

namespace ibcs :: config
{
    class ConfigLoader
    {
        private: 
        RuntimeConfig runtime_; 
        SecurityConfig security_; 
        IntrospectionConfig introspection_; 

        public: 
        bool load_from_yaml(const string &file); 

        const RuntimeConfig &runtime() const {
            return runtime_;  
        }

        const SecurityConfig &security() const {
            return security_; 
        }

        const IntrospectionConfig &introspection() const {
            return introspection_; 
        }
    }; 
}