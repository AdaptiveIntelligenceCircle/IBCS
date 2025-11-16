#include "trace_reader.h"
using namespace std; 

#include <string>
#include <sstream> 

#include <filesystem> 
#include <fstream>

#include "../utils/mini_json.hpp"
namespace fs = filesystem ;
using namespace MiniJson;

namespace ibcs :: trace
{
    TraceReader::TraceReader(const std::string &dir) : directory(dir) {}
    vector<TraceEvent> TraceReader :: load_all()
    {
        vector<TraceEvent> result; 
        for (auto &entry : fs :: directory_iterator(directory)){
            ifstream in(entry.path()) ; 
            stringstream buf; 
            buf << in.rdbuf(); 

            auto arr = parse_string(buf.str()).as_array(); 

            for (auto &item : arr)
            {
                TraceEvent ev; 
                ev.timestamp = item["timestamp"].as_int(); 
                ev.category = item["category"].as_string(); 
                ev.message = item["message"].as_string(); 

                auto meta = item["meta"]; 
                ev.meta.node = meta["node"].as_string(); 
                ev.meta.rule = meta["rule"].as_string(); 

                if (meta.contains("extra")){
                    for (auto &[k, v]: meta["extra"].as_object())
                    ev.meta.extra[k] = v.as_string(); 
                }
                result.push_back(ev); 
            }
        }
        return result; 
    }
    
}