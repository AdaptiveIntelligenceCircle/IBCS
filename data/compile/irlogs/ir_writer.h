#pragma once 
#include "ir_serializer.h"

#include <string> 
using namespace std; 

namespace ibcs :: ir
{
    class IRWriter
    {
        public: 
        explicit IRWriter (const string &dir); 

        // ghi file "transform.ir"
        bool write_log(const IrLog &log); 

        private: 
        string directory; 
        int counter = 0 ; 
    };
}