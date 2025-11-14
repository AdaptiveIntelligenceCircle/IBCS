#pragma once 

#include <string>
#include <vector> 
#include <iostream>

using namespace std; 
#include <algorithm>
#include <memory>

#include <unordered_map>
#include "executor.h"
#include "memory.h"

#include "../core/introspection_engine.h"

using namespace runtime; 

namespace ibcs :: runtime
{
    struct BehaviorContext
    {
        string behavior_id; 
        unordered_map<string, string>parameters;
        string source_module; 
        bool allow_side_effect = false; 
    }; 

    class BehaviorSandBox
    {
        public: 
        BehaviorSandBox(){}; 
        BehaviorSandBox
        (shared_ptr<Executor> exec,
        shared_ptr<Memory>mem, 
        shared_ptr<IntrospectionEngine> introspection 
        ); 

        void loadBehaviorScript(const string &path); 
        string executeBehavior(const BehaviorContext &ctx); 
        void restSandbox(); 

        string getLastOutput() const; 
        const vector<string> &getLogs() const; 

        private: 
        shared_ptr<Executor> executor;
        shared_ptr<Memory> memory; 
        shared_ptr<IntrospectionEngine> introspection; 
        
        vector<string> logs; 
        string last_output;

        void log(const string &entry); 
    }; 
}