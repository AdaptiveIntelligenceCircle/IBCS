#pragma once
#include <iostream>
#include "context_manager.h"
#include "behavior_model.h" 
#include <memory>
#include <vector>
namespace ibcs
{
    enum class ReasoningLevel
    {
        LINEAR, 
        BRANCHED, 
        INTEGRATIVE
    }; 

    struct ReasoningResult
    {
        string conclusion;
        double confidence;
        vector<string> rationale;
    };  

    class MetaReasoner
    {
        public: 
        // string reasonNextAction(const ContextManager &context); 
        MetaReasoner(shared_ptr<ContextManager> context); 
        ReasoningResult infer(const string &Concept, ReasoningLevel level); 
        
        private: 
        shared_ptr<ContextManager> context_manager; 
        ReasoningResult linearInfer(const string& Concept);
        ReasoningResult branchedInfer(const string& Concept);
        ReasoningResult integrativeInfer(const string& Concept);



    };
}
