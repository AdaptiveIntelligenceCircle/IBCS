#pragma once 
#include "trace_event.h"

#include <string> 
#include <vector> 

using namespace std; 

namespace ibcs :: trace
{
    class TraceRecorder
    {   
        private: 
        string directory; 
        vector<TraceEvent> buffer; 
        int file_index  = 0; 
        
        public: 
        explicit TraceRecorder(const string &dir); 

        void record(const TraceEvent &ev); 
        void save(); // flush tat ca ra File Json..// 
    }; 
}