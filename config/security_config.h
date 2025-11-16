#pragma once 
#include <string> 

#include <vector> 
using namespace std; 

namespace ibcs :: config
{
    struct SecurityConfig
    {
        bool allow_remote_introspection = false; 
        vector<string> trusted_nodes; 
        int max_request_size = 1024; 
    }; 
} // namespace ibcs :: config.. 