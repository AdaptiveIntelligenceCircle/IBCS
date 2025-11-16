#pragma once 
#include "ir_serializer.h"

#include <string> 
#include <vector> 

using namespace std ; 

namespace ibcs :: ir
{
    class IRReader
    {
        public: 
        explicit IRReader(const string &dir); 

        vector<IrLog> load_all();
        
        private: 
        string directory; 
    }; 
}