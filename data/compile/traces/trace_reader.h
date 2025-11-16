#pragma once 
#include "trace_event.h"

#include <string> 
#include <vector> 

using namespace std; 

namespace ibcs :: trace
{
    class TraceReader
    {
        public:
        explicit TraceReader(const string &dir); 

        // load tat ca file trace_ *.json.. 
        vector<TraceEvent> load_all(); 

        private: 
        string directory; 
    }; 
}