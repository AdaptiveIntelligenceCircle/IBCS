#include "context_manager.h"
using namespace std;
#include <fstream> 
#include <sstream> 
#include <iostream> 
#include <nlohmann/json.hpp>

using json = nlohmann :: json; 

namespace ibcs
{
    void ContextManager :: setContextValue(const string &key, const string &value)
    {
        context[key] = value; 
        changeLog.push_back("Set global :" + key + "=" + value); 
    }

    void ContextManager :: setValue(const string &scope, const string &key, const string &value)
    {
        scoped[scope][key] = value; 
        changeLog.push_back("Set [" + scope + "]" + key + "=" + value); 
    }

    string ContextManager :: getContextValue(const string &key) const 
    {
        auto it = context.find(key); 
        return it != context.end() ? it -> second : ""; 
    }

    void ContextManager :: clearContext()
    {
        context.clear(); 
        changeLog.push_back("Cleared global context"); 
    }
    
    // ---- Scope Handling ---- /// 
    string ContextManager :: getvalue(const string &scope, const string &key, const string &value)
    {
        auto scopeIt = scoped.find(scope);
        if (scopeIt != scoped.end())
        {
            auto keyIt = scopeIt -> second.find(key); 
            if (keyIt != scopeIt -> second.end())
            {
                return keyIt -> second; 
            }
        } 
        return ""; 
    }

    // --- Snapshot (chua dung stack o day, co the nang cap sau) ---- // 
    void ContextManager :: pushSnapshot()
    {
        changeLog.push_back("Snapshot pushed(not yet saved)"); 
    }

    void ContextManager :: rollback()
    {
        changeLog.push_back("Rollback called (not implemented)");
    }

    // --- Logging --- // 
    void ContextManager :: logChanges()
    {
        cout <<"Context Manager Changelog" << endl; 
        for (const auto &entry : changeLog)
        {
            cout <<"-" << entry << endl; 
        }
    }

    // -- Json Handling -- // 
    void ContextManager :: localFromJson(const string &filepath)
    {
        ifstream in (filepath); 
        if (!in)
        {
            cerr <<"Error : Cannot open file for loading" << filepath << endl;
            return ;  
        }
        json j ; 
        in >> j ; 

        // load global context ..// 
        if (j.contains("context"))
        {
            context = j["context"].get<unordered_map<string, string>>();
        }

        // local scoped ... // 
        if (j.contains("scoped"))
        {
            scoped = j["scoped"].get<unordered_map<string, unordered_map<string, string>>>();
        }

        changeLog.push_back("Loading context from" + filepath); 
    }

    void ContextManager :: saveToJson(const string &filepath)
    {
        ofstream out (filepath);
        if (!out)
        {
            cerr <<"Error : Cannot open file for saving:" << filepath << endl; 
            return ; 
        }

        json j; 
        j["context"] = context; 
        j["scoped"] = scoped; 

        out <<j.dump(4); // pretty print with 4 - space indentation..// 
        changeLog.push_back("Saved context to" + filepath); 
    }
    // string ContextManager :: getContextValue(const string &key) const 
    // {
    //     auto it = context.find(key);
    //     if (it != context.end())
    //     {
    //         return it -> second; 
    //     }
    //     else 
    //     {
    //         return "";  
    //     }
    // }
    // void ContextManager :: clearContext()
    // {
    //     context.clear() ; 
    // }
}
