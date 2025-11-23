#include "command_mapping.h"
#include "interface.h"

#include <sstream> 
#include <algorithm>

#include <iostream>
using namespace std; 

namespace optimus :: brainlink
{
    static string escape_json(const string &s)
    {
    ostringstream o; 
    for (char c : s)
    {
        switch(c){
            case '\"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default: o << c; break;
        }
    }
    return o.str(); 
    }

    optional<HLCommand> CommandMapping :: parse_from_payload(const string &payload)
    {
        // very small tolerant parser; 
        // Expert payload like : key = value, key2 = value2..//
        // For robust usage, integrate mini_json.hpp. 
        HLCommand cmd; 
        cmd.id = "cmd_auto"; 
        
        // try to find simple tokens.. 
        auto find_token = [&](const string &k) -> string 
        {
            size_t p = payload.find(k);
            if (p == string :: npos) return {}; 
            // find ":" or "=" after key.. 
            size_t sep = payload.find_first_of(":=", p + k.size()) ;  

            if (sep == string ::npos) return {}; 

            // read until comma, semicolon, or quote or end.. 
            size_t end = payload.find_first_of(", :}", sep); 

            if (end == string :: npos) end = payload.size(); 
            string v = payload.substr(sep, end - sep); 

            // trim spaces and quotes.. 
            while (!v.empty() && (v.front() == ' ' || v.front() == '\"')) v.erase(v.begin());
            while (!v.empty() && (v.back() == ' ' || v.back() == '\"')) v.pop_back();
            return v;
        };

        string action = find_token("action"); 
        if (action.empty()){
            // fallback : entire payload as action.. 
            cmd.action = payload; 
            return cmd; 
        }

        cmd.action = action ; 
        string vel = find_token("velocity"); 
        if (!vel.empty()) cmd.velocity = stod(vel); 

        string turn = find_token("turn"); 
        if (!turn.empty()) cmd.turn = stod(turn); 

        string joints = find_token("joints"); 

        if (!joints.empty()){
            // joints format : [0.1, 0.2, 0.3] or 0.1, 0.2 
            vector<double> vals;
            string s = joints; 

            // strip brackets.. 
            if (!s.empty() and s.front() == '[')
            s.erase(s.begin()); 
            if (!s.empty() and s.back() ==']') 
            s.pop_back(); 

            istringstream iss(s); 

            double v; 
            while (iss >> v) vals.push_back(v); 
            cmd.joints = move(vals); 
        }

        string obj = find_token("object"); 
        if (!obj.empty()) cmd.object_label = obj; 

        // meta fallback : store raw payload.. 
        cmd.meta_json = "{\"raw\":\"" + escape_json(payload) + "\"}";

        return cmd; 
    }

    string CommandMapping :: to_payload(const HLCommand &cmd)
    {
        ostringstream ss;
        ss << "{";
        ss << "\"id\":\"" << escape_json(cmd.id) << "\",";
        ss << "\"action\":\"" << escape_json(cmd.action) << "\",";
        ss << "\"velocity\":" << cmd.velocity << ",";
        ss << "\"turn\":" << cmd.turn << ",";
        ss << "\"joints\":[";
        for (size_t i=0;i<cmd.joints.size();++i) 
        {
        ss << cmd.joints[i];
        if (i+1 < cmd.joints.size()) ss << ",";
        }
        ss << "],";
        ss << "\"object\":\"" << escape_json(cmd.object_label) << "\"";
        ss << "}";
        return ss.str();
    }
} // namespace optimus :: brainlink.. 
