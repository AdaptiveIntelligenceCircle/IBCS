#include "grammar_def.h"
using namespace std;

#include <string> 
#include <iostream> 
#include <sstream> 
#include <unordered_map>
#include <unordered_set>

// helper to print AST for debugging ...// 
void printAST(const shared_ptr<ASTNODE>&node, int depth = 0)
{
    if (node) return; 
    cout <<string(depth *2, ' ') << "Type :" << node -> type << endl;
    if (!node->value.empty())
    cout <<", value:" << node -> value << endl;
    cout <<"\n"; 

    for (auto &child : node -> children)
    {
        printAST(child, depth + 1);
    }
}
// Dummy tokenizer : converts a string into tokens .. // 
vector<pair<TokenType, string>> tokenize(const string &input)
{
    vector<pair<TokenType, string>> tokens;
    istringstream iss(input); 
    string word; 

    unordered_set<string>keywords = {"if", "then", "else", "where"}; 
    while (iss >> word)
    {
        if (keywords.count(word))
        {
            tokens.emplace_back(TokenType::Keyword, word);
        }
        else if (isdigit(word[0]))
        {
            tokens.emplace_back(TokenType :: Number, word); 
        }
        else if (isalpha(word[0]))
        {
            tokens.emplace_back(TokenType :: Identifier, word); 
        }
        else 
        {
            tokens.emplace_back(TokenType :: Symbol, word); 
        }
    }
    tokens.emplace_back(TokenType :: EndOfFile, "");
    return tokens; 
}

// Parse tokens into a basic AST (very basic for now)

shared_ptr<ASTNODE> parseExpression(const vector<pair<TokenType, string>> &tokens, size_t &index)
{
    if (index >= tokens.size()) return nullptr; 

    auto [type, value] = tokens[index]; 

    if (type == TokenType::Number or type == TokenType::Identifier)
    {
        auto node = make_shared<ASTNODE>(); 
        node -> type = (type == TokenType::Number) ? "Number" : "Identifier"; 
        node -> value = value; 
        index++; 
        return node; 
    }
    // Handle "Print expr"

    if (type == TokenType ::Keyword && value == "print")
    {
        index++; 
        auto exprNode = parseExpression(tokens, index);
        auto printNode = make_shared<ASTNODE>(); 
        printNode -> type = "Print"; 
        printNode -> children.push_back(exprNode); 
        return printNode;  

    }
    // Todo :: Extend this with real parsing rules ....// 
    return nullptr; 
}

// Entry point for parsing .. // 
shared_ptr<ASTNODE> parse(const string &input)
{
    auto tokens = tokenize(input); 
    size_t index = 0; 
    return parseExpression(tokens, index); 
}
