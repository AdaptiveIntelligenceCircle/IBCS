#include <iostream> 
using namespace std; 

#include <memory> 
#include "grammar_def.h"
class Interpreter
{
    public: 
    void execute(const shared_ptr<ASTNODE> &node);
    void eval(const shared_ptr<ASTNODE> &node); 
    
    private:
    GrammarRule env;
}; 
