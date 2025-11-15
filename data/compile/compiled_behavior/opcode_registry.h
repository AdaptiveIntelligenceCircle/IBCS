#pragma once 
#include "compile_context.h"

#include <string>
#include <cmath> 

using namespace std; 

namespace ibcs :: compile
{
    class OpcodeRegistry{
        public: 
        static int resolve_id(const CompileContext &ctx, const string &name); 
    }; 
}