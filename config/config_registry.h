#pragma once 
#include "runtime_config.h"

#include "security_config.h"
#include "introspection_config.h"

namespace ibcs :: config
{
    class ConfigRegistry
    {
        public: 
        static void set(const RuntimeConfig &f); 
        static void set(const SecurityConfig &s); 
        static void set(const IntrospectionConfig &i); 

        static const RuntimeConfig &runtime(); 
        static const SecurityConfig &security(); 
        static const IntrospectionConfig &introspection(); 

        private: 
        static RuntimeConfig runtime_; 
        static SecurityConfig security_; 
        static IntrospectionConfig introspection_;  
    }; 
}