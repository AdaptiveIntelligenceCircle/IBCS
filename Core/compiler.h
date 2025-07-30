#include <iostream>
#pragma once
#include <string> 

#include "behavior_model.h"
#include "context_manager.h"

using namespace std;

namespace ibcs
{
    class Compiler
    {
        public: 
        Compiler(); 

        // Bien dich hanh vi (tu model) thanh logic cu the...// 
        string compileBehavior(const BehaviorModel &model, const ContextManager &context ); 

        private: 
        void optimizeCompiledCode(string &code); 
    }; 
}
