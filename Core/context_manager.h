#pragma once 
#include <iostream>
#include <unordered_map> 
#include <string>
#include <vector> 
using namespace std; 

namespace ibcs
{
    class ContextManager
    {
        public: 
        void setContextValue(const string &key, const string &value);
        string getContextValue(const string &key) const;  
        void clearContext(); 
        void setValue(const string &scope, const string &key, const string &value); 
        string getvalue(const string &scope, const string &key, const string &value);
        void pushSnapshot();  
        void rollback(); 
        void logChanges(); 
        void localFromJson(const string &filepath); 
        void saveToJson(const string &filepath);
       

        private: 
        unordered_map<string, string> context;
        unordered_map<string, unordered_map<string, string>> scoped; 
        vector<string>changeLog;
    } ; 
}
