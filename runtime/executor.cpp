#include "executor.h"
using namespace std; 

#include <iostream>

namespace runtime
{
    Executor :: Executor(shared_ptr<Memory> mem)
    : memory(move(mem)){}

    void Executor :: register_behavior(const string &name, 
    function<void(const ExecutionContext&)> handler)
    {
        behavior_map[name] = handler; 
    }

    void Executor :: enable_debug(bool flag)
    {
        debug_mode = flag; 
    }

    void Executor :: log(const string &msg)
    {
        if (debug_mode)
        {
            cout <<"Executor :" << msg << endl; 
        }
    }

    void Executor :: execute(const ExecutionContext &ctx)
    {
        log("Executing Behavior " + ctx.behavior_id);

        auto it = behavior_map.find(ctx.behavior_id); 
        if (it != behavior_map.end())
        {
            try 
            {
                it -> second(ctx); 
            } catch(const exception &e){
                log("Exception in behavior:" + string (e.what())); 
            }
        
        }
        else 
        {
            log("Unknown behavior :" + ctx. behavior_id); 
        }

        if (ctx.introspective)
        {
            log("Triggering introspection for behavior:" + ctx. behavior_id); 
            // Giao tiep voi introspection engine .. // 
        }
    }
}
// namespace runtime. . 
