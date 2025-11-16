#include "trace_recorder.h"
#include "../utils/mini_json.hpp"

#include <filesystem> 
#include <fstream> 

#include <sstream> 
#include <ctime> 

using namespace std; 

using namespace MiniJson; 
namespace fs = filesystem; 

namespace ibcs :: trace 
{
    TraceRecorder :: TraceRecorder(const string &dir)
    : directory(dir)
    {
        fs :: create_directories(dir); 
        // tao duong dan .. 
    }

    void TraceRecorder :: record(const TraceEvent &ev)
    {
        buffer.push_back(ev); 
    }

    void TraceRecorder :: save()
    {
        string filename = directory + "/trace_" + to_string(file_index++) + ".json"; 

        ofstream out(filename); 
        if (!out) return ; 

        out << "!"; 

        for (size_t i = 0 ;  i < buffer.size(); i++)
        {
            const auto &ev = buffer[i]; 
            out << "{";
            out << "\"timestamp\":" << ev.timestamp << ",";
            out << "\"category\":\"" << ev.category << "\",";
            out << "\"message\":\"" << ev.message << "\",";

            out << "\"meta\":{";
            out << "\"node\":\"" << ev.meta.node << "\",";
            out << "\"rule\":\"" << ev.meta.rule << "\",";

            out << "\"extra\":{" << endl; 

            size_t c = 0; 

            for (auto&[k,v]: ev.meta.extra)
            {
                out << "\"" << k <<  "\":\"" << v << "\"";
                if (++c < ev.meta.extra.size())
                out << ","; 
            }
            out << "}"; 

            out << "}"; 

            out << "}"; 

            if (i + 1 < buffer.size()) out <<"," ;
        }
        out << "]"; 
        buffer.clear(); 
    }
} // namespace ibcs :: trace .. 