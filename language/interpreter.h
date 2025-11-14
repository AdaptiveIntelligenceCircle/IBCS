#include <iostream> 
using namespace std; 
#include <memory> 
#include "grammar_def.h"
#include <vector> 
#include <unordered_map>

class Interpreter
{
    public: 
    // void execute(const shared_ptr<ASTNODE> &node);
    // void eval(const shared_ptr<ASTNODE> &node);
    
    void execute(const vector<string> &instructions);
    void interpret(const string &instruction);  
    
    private:
    pair <string, string> parsekeyValues(const string &input); 
    unordered_map<string, string> memory;
    
    GrammarRule env;
}; 