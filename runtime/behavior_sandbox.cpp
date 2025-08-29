#include <iostream>

#include "behavior_sandbox.h"

using namespace std ; 
#include <iostream> 
#include <fstream>

namespace ibcs :: runtime
{
    BehaviorSandBox :: BehaviorSandBox(shared_ptr<Executor> exec, 
    shared_ptr<Memory> mem, shared_ptr<IntrospectionEngine> introspection)
    {

    }

    void BehaviorSandBox::loadBehaviorScript(const string &path)
    {
        ifstream file(path); 
        if (!file)
        {
            log("Error - Cannot open behavior script at" + path); 
            return; 
        }
        string script(istreambuf_iterator<char>(file), istreambuf_iterator<char>()); 
        // executor -> loadScript(script);
        log("[info] - behavior script loaded:" + path);  
    }

    string BehaviorSandBox :: executeBehavior(const BehaviorContext &ctx)
    {
        log("Info - Executing behavior:" + ctx.behavior_id); 

        // push context into memory space .. // 
        memory -> createIsolatedScope(ctx.behavior_id);
        for (const auto &[k, v] : ctx.parameters)
        {
            memory -> set(ctx.behavior_id, k); 
        } 

        // pre - intropection.s
        // introspection -> recordPreExecution(ctx.behavior_id, ctx.parameters);
         
        string result = executor -> run(ctx.behavior_id);
        // last_output = executor -> getLastResult(); 

        if (!ctx.allow_side_effect)
        {
            memory -> exists(ctx.behavior_id); 
            log("Info Rolledback memory changes for behavior" + ctx.behavior_id); 
        }else 
        {
            memory -> get(ctx.behavior_id); 
            log("Committed memory changes for behavior:" + ctx.behavior_id);
        }
        return result;
    }

    void BehaviorSandBox :: restSandbox()
    {
        // executor -> reset(); 
        // memory -> clear(); 
        logs.clear(); 
        last_output.clear(); 
        log("Info Sanbox enviroment reset"); 
    }

    string BehaviorSandBox :: getLastOutput() const 
    {
        return last_output; 
    }

    const vector<string> &BehaviorSandBox :: getLogs() const 
    {
        return logs; 
    }

    void BehaviorSandBox :: log(const string &entry)
    {
        logs.emplace_back(entry); 
        cout << entry << endl;
    }
}
