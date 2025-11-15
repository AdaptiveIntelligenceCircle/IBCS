#pragma once 

#include "compile_context.h"
using namespace std; 

namespace ibcs :: compile
{
    class RuleLoader
    {
        public: 
        static bool validate(const CompileContext &ctx, const string &node_type,  
        const vector<string> &children); 
        
    }; 
}