#include <iostream>

using namespace std; 

#ifndef IBCS_RUNTIME_MEMORY_H 
#define IBCS_RUNTIME_MEMORY_H

#include <iostream>
using namespace std; 

#include <unordered_map>
#include <string>

#include <variant>
#include <vector> 
#include <mutex> 

namespace ibcs :: runtime
{
    // bien kieu value ho tro cac loai du lieu co ban ..// 
    using Value = variant<int, double, bool, string>;

    // Cau truc dai dien cho mot doi tuong trong memory (symbol, object,...)..// 
    struct MemoryObject
    {
        Value data; 
        bool readOnly = false; // co cho phep ghi de ? 
        bool persistent = false; // Co luu qua cac chu ky execution? 
        string behavior_id; 
    }; 

    class Memory
    {
        public: 
        Memory(); 

        // truy xuat ..// 
        bool exists(const string &name) const; 
        Value get(const string &name) const; 

        // Gan gia tri .. //
        void set(const string &name, const Value &value); 
        void define(const string &name, const Value &value, bool readOnly, bool persistent); 

        // Xoa hoac lam sach ,.. // 
        void remove(const string &name); 
        void clearNonPersisten(); 
        string createIsolatedScope(string behavior_id);

        // Debugging and introspection..// 
        vector<string> listVariables() const; 
        string dumpState() const; 

        private: 
        unordered_map<string, MemoryObject> memory_; 
        mutable mutex mutex_; 
        // Cho sandbox/thread - safe access...// 
    };
}

#endif 

