#include "ir_serializer.h"
#include "../utils/mini_json.hpp"
#include <sstream>
#include <ctime> 

using namespace std; 

namespace ibcs :: ir
{
    using namespace MiniJson; 

    string IRSerializer :: serializer(const IrLog &log)
    {
        ostringstream oss ; 
        oss <<"{"; 
        oss << "\"timestamp\":" << log.timestamp << ",";
        oss << "\"stage\":\"" << log.stage << "\",";
        oss << "\"input\":\"" << log.input << "\",";
        oss << "\"output\":\"" << log.output << "\",";

        oss << "\"meta\":{";
        oss << "\"node_count\":" << log.meta.node_count << ",";
        oss << "\"opcode_count\":" << log.meta.opcode_count << ",";
        oss << "\"rules_applied\":[";

        for (size_t i = 0 ; i< log.meta.rules_applied.size(); i++)
        {
            oss <<"\"" << log.meta.rules_applied[i] << "\""; 
            if (i + 1 < log.meta.rules_applied.size())
            oss <<","; 
        }

        oss  << "]"; 
        oss  << "}"; 

        oss << "}"; 

        return oss.str(); 
    }

    IrLog IRSerializer :: deserialize(const string &json_str)
    {
        IrLog log; 
        auto v = parse_string(json_str); 

        log.timestamp = v["timestamp"].as_int(); 
        log.stage = v["stage"].as_string(); 
        log.input = v["input"].as_string(); 
        log.input = v ["output"].as_string(); 

        auto meta = v["meta"]; 
        log.meta.node_count = meta["node_count"].as_int() ; 
        log.meta.opcode_count  = meta["opcode_count"].as_int(); 

        for (auto &item : meta["rules_applied"].as_array())
        log.meta.rules_applied.push_back(item.as_string()); 

        return log; 
    }
} // namespace ibcs :: ir .. // 