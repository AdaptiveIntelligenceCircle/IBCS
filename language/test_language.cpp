#include <iostream> 
#include <memory>
#include "grammar_def.h"
#include "transformer.h"

#include "transformer.cpp"
#include "grammar_def.cpp"

using namespace std; 
using namespace ibcs; 

int main ()
{
    cout <<"[TEST] Language module intergration test\n"; 

    // === Case 1 : simple number ==== = // 
    {
        string code = "42"; 
        auto ast = parse(code); 
        cout <<"\n[Case1] Input:" << code << endl; 
        printAST(ast); 
        Transformer t; 
    }

    // === Case 2 : identifier === // 

    {
        string code = "x"; 
        auto ast = parse(code); 
        cout <<"\n[Case2 Input]:" << code << endl; 
        printAST(ast); 
    }

    // === Case 3 : print expression === // 

    {
        string code = "print 99";
        auto ast = parse(code); 
        cout <<"\n[Case3 Input] :" << code << endl; 
        printAST(ast);  
        Transformer t; 
    }

    /// === Case 4 : Fake assignment manual AST ====// 
    {
        cout <<"\n [Case 4] Manual AST for Assignment:" << endl ;
        auto assignNode = make_shared<ASTNODE>(); 
        assignNode -> type = "Assignment" ; 
        
        auto varNode = make_shared<ASTNODE>(); 
        varNode -> type = "Identifier";  
        varNode -> value = "x"; 

        auto valNode = make_shared<ASTNODE>(); 
        valNode -> type = "Number"; 
        valNode -> value = "10";
        
        assignNode -> children.push_back(varNode); 
        assignNode ->children.push_back(valNode);

        Transformer t; 
        ///t.transform(assignNode); 
    }

    // Case 5 : fake function call (manual ast)

    {
        cout <<"\n [Case 5] : Manual AST for FunctionCall\n";  
        auto funcNode = make_shared<ASTNODE>(); 
        funcNode -> type = "FunctionCall"; 
        funcNode -> value = "foo"; 

        auto arg1 = make_shared<ASTNODE>(); 
        arg1 -> type = "Number"; 
        arg1 -> value = "1"; 

        auto arg2 = make_shared<ASTNODE>(); 
        arg2 -> type = "Identifier"; 
        arg2 -> value = "x"; 

        funcNode -> children.push_back(arg1); 
        funcNode -> children.push_back(arg2); 

        Transformer t; 
        // t.transform(funcNode); 
    }

    cout <<"\nTest - Finished all case" << endl; 
    return 0; 
}