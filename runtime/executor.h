#ifndef EXECUTOR_H
#define EXECUTOR_H 
#include <iostream> 
using namespace std; 

#include <string> 
#include <unordered_map>

#include <memory> 
#include <functional> 

#include "memory.h"

using namespace ibcs :: runtime;

namespace runtime
{
    struct ExecutionContext
    {
        string behavior_id; 
        unordered_map<string, string> parameters;
        int priority = 0; 
        bool introspective = false; 
    };

    class Executor
    {
        public: 
       
        Executor(shared_ptr<Memory> mem);
        
        // main interface .. ..
        void execute(const ExecutionContext &ctx); 

        // Register a native behavior handler..
        void register_behavior(const string &name, function<void(const ExecutionContext&)> handler); 

        // Enable/disable logging or intropection..// 
        void enable_debug(bool flag);

        string run(string behavior_id); 
        // string loadScript(string &script); 
        private: 
        unordered_map<string, function<void(const ExecutionContext&) >> behavior_map; 
        shared_ptr<Memory> memory;  
        bool debug_mode = false; 

        void log(const string &msg);
    }; 
} // namespace runtime ..// 



#endif 
