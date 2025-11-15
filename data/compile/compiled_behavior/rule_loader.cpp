#include "rule_loader.h"
using namespace std; 

#include <iostream> 
#include <string> 

namespace ibcs :: compile
{
    bool RuleLoader :: validate(const CompileContext &ctx
    , const string &node, const vector<string> &children)
    {
        for (const auto &rule : ctx.get_rules()){
            if (rule.node!= node) continue; 
            // check allowed children.. 
            for (const auto &c : children){
                if (!rule.allowed_children.empty() &&
                find(rule.allowed_children.begin(),
                          rule.allowed_children.end(),
                          c) == rule.allowed_children.end()) {
                return false;
                }
            }
            return true; 
        }
        return true; // no rule = allow 
    }
} // namespace ibcs :: compile .. 
