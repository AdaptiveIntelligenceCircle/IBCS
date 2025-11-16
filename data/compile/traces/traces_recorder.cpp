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
}