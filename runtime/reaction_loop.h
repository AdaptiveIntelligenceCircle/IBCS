#pragma once 

#include <iostream>
using namespace std; 

#include <memory>
#include <thread>

#include "behavior_sandbox.h"
#include "memory.h"
#include "executor.h"

namespace ibcs :: runtime
{
    class ReactionLoop
    {   
        protected: 
        shared_ptr<BehaviorSandBox> sandbox; 
        shared_ptr<Executor> executor; 
        shared_ptr<Memory> memory;

        atomic <bool> running; 
        int interval_ms ; 
        thread loop_thread; 

        void loop(); 

        public: 

        ReactionLoop(shared_ptr<BehaviorSandBox> sb, 
        shared_ptr <Executor> ex, 
        shared_ptr<Memory> mem, int interval = 100);
        
        ~ReactionLoop();

        void start(); 
        void stop(); 
        bool isRunning() const 
        {
            return running.load(); 
        }
    };
} // namespace :: runtime ;  

