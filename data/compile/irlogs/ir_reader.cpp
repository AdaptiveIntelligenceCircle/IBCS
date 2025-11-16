#include "ir_reader.h"
#include <filesystem> 

#include <fstream> 
#include <iostream>

using namespace std; 

namespace fs = filesystem; 
namespace ibcs :: ir 
{
    IRReader :: IRReader(const string &dir) : directory(dir){}

    vector<IrLog> IRReader :: load_all()
    {
        vector<IrLog> logs;

        for (auto &entry :fs ::directory_iterator(directory)){
            if (entry.path().extension() == ".ir"){
                ifstream in(entry.path()); 
                stringstream buf;
                buf << in.rdbuf(); 
                logs.push_back(IRSerializer :: deserialize(buf.str())); 
            }
        } 
        return logs; 
    }
} // namespace ibcs :: ir 