#pragma once 
#include <iostream>
#include <string>
#include <vector>

using namespace std; 

namespace ibcs
{
    class IntrospectionEngine
    {
        public: 
        void analyzeThoughtProcess(const vector<string> &ideastream);
        string summarizeIntropection(); 
        
        private: 
        vector<string> innerThoughts;
    };  
}
