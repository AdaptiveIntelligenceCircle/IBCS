#include "reaction_loop.h"
using namespace std; 

#include <string>
#include <algorithm>  

#include <chrono>
using namespace ibcs :: runtime; 

ReactionLoop :: ReactionLoop(shared_ptr<BehaviorSandBox> sb, 
    shared_ptr<Executor> ex , shared_ptr<Memory> mem, int interval
) : sandbox(sb), executor(ex), memory(mem), running(false), interval_ms(interval)
{}

ReactionLoop :: ~ReactionLoop()
{
    stop();
}

void ReactionLoop :: start()
{
    if (running.load())
    {
        cerr <<"[ReactionLoop] - Already running" << endl; 
        return;
    }
    running.store(true);
    loop_thread = thread(&ReactionLoop :: loop, this); 
}

void ReactionLoop :: stop()
{
    if (!running.load()) return;
    running.store(false); 
    if (loop_thread.joinable())
    {
        loop_thread.join(); 
    }
}

void ReactionLoop :: loop()
{
    cout <<"[ReactionLoop] - Started loop with interval" << interval_ms << "ms\n";

    while (running.load())
    {
        // todo : tich hop event queue, trigger tu sandbox hoac executor ....//
        cout <<"[ReactionLoop] Tick\n"; 
        this_thread :: sleep_for(chrono ::milliseconds(interval_ms)); 
    }

    cout <<"[Reaction LOOP] loop stopped" << endl; 
}
