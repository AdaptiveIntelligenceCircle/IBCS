#include "memory.h"
using namespace std; 

#include <sstream>
#include <stdexcept>

namespace ibcs :: runtime
{
    Memory :: Memory() = default; 

    bool Memory :: exists(const string &name) const 
    {
        lock_guard <mutex> lock(mutex_); 
        return memory_.find(name) != memory_.end();
    }

    Value Memory :: get(const string &name) const 
    {
        lock_guard <mutex> lock(mutex_); 
        auto it = memory_.find(name); 
        if (it == memory_.end())
        {
            throw runtime_error("Variable'" + name + "'not found."); 
        }
        return it -> second.data; 
    }

    void Memory :: set(const string &name, const Value &value)
    {
        lock_guard<mutex> lock(mutex_); 
        auto it = memory_.find(name); 

        if (it == memory_.end())
        {
            throw runtime_error("'Variable" + name + "'not declared"); 
        }
        if (it -> second.readOnly)
        {
            throw runtime_error("'Variable" + name +"'is Readonly");
        }
        it -> second.data = value;
    }

    void Memory :: define(const string &name, const Value &value, bool readOnly, bool persistent)
    {
        lock_guard<mutex> lock(mutex_); 
        memory_[name] = MemoryObject{value, readOnly, persistent};
    }

    void Memory :: remove(const string &name)
    {
        lock_guard<mutex> lock(mutex_); 
        memory_.erase(name); 
    }

    void Memory :: clearNonPersisten()
    {
        lock_guard<mutex> lock(mutex_); 
        for (auto it = memory_.begin() ; it != memory_.end();)
        {
            if (!it -> second.persistent)
            {
                it = memory_.erase(it); 
            }
            else 
            {
                ++it;
            }
        } 
    }
    
    vector<string> Memory :: listVariables() const 
    {
        lock_guard<mutex> lock(mutex_); 
        vector<string> names;
        for (const auto &pair : memory_)
        {
            names.push_back(pair.first); 
        }
        return names;
    }

    string Memory :: dumpState() const 
    {
        lock_guard<mutex> lock(mutex_); 
        ostringstream oss; 
        for (const auto &[key, obj]: memory_)
        {
            oss << key <<"="; 
            visit([&](auto &&val)
            {
                oss << val;
            }, obj.data);
            oss << (obj.readOnly ? " [readonly]" : "") << (obj.persistent ? " [persistent]" : "") << "\n";
        }
        return oss.str(); 
    }
}
