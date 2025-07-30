#include "idea_parser.h"

#include <regex> 
#include "idea_parser.h"
using namespace std; 
#include <vector> 

namespace ibcs
{
    ParsedIdea :: ParsedIdea(const string &input, const string &c, const vector<string> &m, double conf)
    : raw_input(input), Concept(c), modifiers(m), confidence(conf){}

    IdeaParser :: IdeaParser(shared_ptr<ContextManager> context)
    : context_manager(context){}
    
    ParsedIdea IdeaParser :: parse(const string &raw_data)
    {
        string Concept = extractConcept(raw_data); 
        vector<string> modifiers = extractModifiers(raw_data); 
        double confidence = estimateConfidence(raw_data); 

        ParsedIdea idea(raw_data, Concept, modifiers, confidence); 
        updateContext(idea); 
        return idea; 
    }

    string IdeaParser :: extractConcept(const string &input)
    {
        regex concept_pattern (R"(#([a-zA-Z_]+))");
        smatch match; 
        if (regex_match(input, match, concept_pattern)){
            return match[1]; 
        }
        return "unknown_concept"; 
    }

    vector<string> IdeaParser :: extractModifiers(const string &input)
    {
        regex modifier_pattern(R"(#([a-zA-Z_]+))"); 
        sregex_iterator begin(input.begin(), input.end(), modifier_pattern);
        sregex_iterator end;

        vector<string> modifiers; 
        for (auto it = begin; it!= end; ++it)
        {
            modifiers.push_back((*it)[1]);
        }
        return modifiers;
    }

    double IdeaParser::estimateConfidence(const string& input) 
    {
    if (input.find("?") != string::npos) return 0.3;
    if (input.find("!") != string::npos) return 0.95;
    return 0.7;
    }

    void IdeaParser::updateContext(const ParsedIdea& idea) 
    {
    // context_manager->recordIdea(idea.Concept, idea.confidence);
    }
}
