#pragma once 
#include <string> 
#include <unordered_map>
#include <vector> 

#include <memory> 

using namespace std; 

namespace ibcs :: compile
{
    struct OpcodeDef
    {
        int id; 
        int args;
    }; 

    struct RuleEntry
    {
        string node; 
        vector<string> allowed_children; 
        int max_args = -1; 
    };  

    class CompileContext
    {
        public : 

        bool load_all(const string &dir); 

        // opcode
        const OpcodeDef *get_opcode(const string &name) const; 

        // AST rules.. 
        const vector<RuleEntry> &get_rules() const 
        {
            return get_rules(); 
        }

        // introspection mapping .. 
        string lookup_introspection(const string &command) const ; 

        private: 
        unordered_map<string, OpcodeDef> opcode_table; 
        vector<RuleEntry> rules_; 
        unordered_map<string, string> introspection_map_; 
    }; 

}