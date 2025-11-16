#include "ir_writer.h"
#include <fstream> 

#include <filesystem> 
using namespace std; 

namespace fs = filesystem; 

namespace ibcs :: ir
{
    IRWriter :: IRWriter(const string &dir) : directory(dir)
    {
        fs :: create_directories(dir) ; 
    }

    bool IRWriter :: write_log(const IrLog &log)
    {
        string filename  = directory + "/log_" + to_string(counter);

        ofstream out(filename); 
        if (!out) return false; 

        out <<IRSerializer :: serializer(log); 
        return true; 
    }
}