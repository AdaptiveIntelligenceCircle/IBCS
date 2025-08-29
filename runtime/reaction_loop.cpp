#include "reaction_loop.h"
using namespace std; 

#include "executor.h"
#include "memory.h"

#include "behavior_sandbox.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace ibcs ::runtime; 

namespace runtime
{
    void ReactionLoop :: start()
    {
        running = true; 
        cout <<"Reaction Loop - Starting main reaction loop...\n" << endl; 

        while(running)
        {
            auto inputContext = memory -> fetchLastestInput();
            if (inputContext)
            {
                auto reactionPlan = sandbox -> analyze(inputContext.value()); 
                if (reactionPlan.isValid())
                {
                    executor -> execute(reactionPlan); 
                }
                else 
                {
                    cout <<"Reaction - No loop reaction plan generated" << endl; 
                }
            }
            else{
                cout <<"Reaction Loop - No new input context Available" << endl; 
            }
            // this_thread :: sleep_for(chrono::milliseconds(loopThread)); 
        }
        cout <<"Reaction Loop - stopped " << endl; 
    }
    void ReactionLoop :: stop()
    {
        running = false; 
    }

    
}

