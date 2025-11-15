#include "compile_context.h"
#include <fstream>

#include <iostream>
#include "../utils/mini_json.hpp"

using namespace std; 

namespace ibcs :: compile
{
    bool CompileContext :: load_all(const string &dir)
    {
        // 1> load opcode_def.json.. 
        {
            ifstream f (dir + "/opcode_def.json"); 
            if (!f) return false; 

            auto json = MiniJson :: parse(f); 

            auto ops = json["opcodes"].as_object(); 
            for (auto &[name, val] : ops){
                OpcodeDef def{
                    val["id"].as_int(), 
                    val["args"].as_int()
                };
                opcode_table[name] = def;
            }
        }
        // 2> load ast rules.json.. 
        {
            ifstream f (dir +"/ast_rules.json"); 
            if (!f) return false; 

            auto json = MiniJson :: parse(f); 
            for (auto &r : json["rules"].as_array()){
                RuleEntry entry; 
                entry.node = r["node"].as_string(); 

                if (r.contains("Allowed_children"))
                {
                    for (auto &child : r["allowed_children"].as_array())
                    entry.allowed_children.push_back(child.as_string()); 
                }
                if (r.contains("max_args"))
                entry.max_args = r["max_args"].as_int(); 

                rules_.push_back(entry) ; 
            }
        }

        // 3> Load introspection_map.json..
        {
            ifstream f(dir + "/introspection_map.json"); 
            if (!f) return false; 

            auto json = MiniJson :: parse(f); 
            for (auto &[key, val] : json.as_object()){
                introspection_map_[key] = val.as_string();
            }
        }
        return true; 
    }

    const OpcodeDef *CompileContext :: get_opcode(const string &name) const 
    {
        auto it = opcode_table.find(name); 
        return (it == opcode_table.end()) ? nullptr : &it -> second;
    }

    string CompileContext ::lookup_introspection(const string &cmd) const 
    {   
        auto it = introspection_map_.find(cmd); 
        return (it == introspection_map_.end()) ? "" : it -> second; 
    }
} // namespace ibcs :: compile..// 

