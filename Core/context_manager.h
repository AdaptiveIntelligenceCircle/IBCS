#pragma once 
#include <iostream>
#include <unordered_map> 
#include <string>

using namespace std; 

namespace ibcs
{
    class ContextManager
    {
        public: 
        void setContextValue(const string &key, const string &value);
        string getContextValue(const string &key) const;  
        void clearContext(); 
        

        private: 
        unordered_map<string, string> context; 
    } ; 
}
