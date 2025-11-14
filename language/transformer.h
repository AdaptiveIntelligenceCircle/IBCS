#pragma once 
#include <iostream>
#include <string> 
#include <algorithm>
#include <memory>
#include <unordered_map>
#include "grammar_def.h"

using namespace std;

namespace ibcs
{
    class Transformer
    {
        public:
        void transform(const shared_ptr<ASTNODE>&node);
        void transformAssignment(const shared_ptr<ASTNODE> &node); 
        void transformExpression(const shared_ptr<ASTNODE> &node); 
        void transformFunctionCall(const shared_ptr<ASTNODE> &node); 
        string evaluateExpression(const shared_ptr<ASTNODE>&node); 

        Transformer(){};
        ~Transformer() = default ;
        // const std::unordered_map<std::string, std::string>& getSemanticMap() const;
        unordered_map<string, string> semanticMap; 
    }; 
}
