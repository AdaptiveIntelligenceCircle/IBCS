#include <iostream>
using namespace std; 
#include <vector>
#include <memory>
enum class TokenType
{
    Identifier, 
    Number,
    String, 
    Keyword, 
    Symbol, 
    EndOfFile,
    // .... // 
};

struct GrammarRule
{
    string name; 
    vector<string> production; 
}; 

struct ASTNODE
{
    string type;
    vector<shared_ptr<ASTNODE>> children;
    string value;
}; 
