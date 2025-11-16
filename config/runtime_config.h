#pragma once 
#include <iostream> 

using namespace std; 
#include <string> 

namespace ibcs :: config
{
    struct RuntimeConfig
    {
        string log_level = "info"; 
        int max_threads = 4; 
        bool hot_heated  = false; 
    }; 
} // namespace ibcs :: config.. // 