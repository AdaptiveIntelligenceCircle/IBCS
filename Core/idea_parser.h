#pragma once
#include <iostream>
#include <string> 
#include <memory> 

#include "behavior_model.h"
#include "context_manager.h"
using namespace std; 

namespace ibcs
{
    struct ParsedIdea
    {
        string raw_input;
        string Concept; 
        vector<string> modifiers; 
        double confidence;

        ParsedIdea(const string &input, const string &c, const vector<string> &m, double conf);

    }; 

    class IdeaParser
    {
        public: 
        BehaviorModel parseRawIdea(const string &rawText);
        ParsedIdea parse(const string &raw_idea); 
        void updateContext(const ParsedIdea &idea); 

        IdeaParser(shared_ptr<ContextManager> context);
        
        private: 
        shared_ptr<ContextManager> context_manager;

        string extractConcept(const string &input); 
        vector<string> extractModifiers(const string &input);
        double estimateConfidence(const string &input);  
    };
}
