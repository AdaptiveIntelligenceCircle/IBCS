#pragma once 
#include <string> 
#include <vector> 
#include <unordered_map>
#include <iostream> 

using namespace std; 

namespace ibcs :: ir
{
    struct IRMeta
    {
        int node_count = 0; 
        int opcode_count = 0; 
        vector<string> rules_applied; 
    }; 

    struct IrLog{
        uint64_t timestamp = 0; 
        string stage; 
        string input; 
        string output; 
        IRMeta meta; 
    }; 

    // Serilize. Deserialize IRLOG.. 
    class IRSerializer
    {
        public: 
        static string serializer(const IrLog &log);
        static IrLog deserialize(const string &json_str); 
    }; 
}