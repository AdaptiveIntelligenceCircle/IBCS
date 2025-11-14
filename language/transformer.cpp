#include <iostream> 
using namespace std;
#include <vector> 
#include "transformer.h"

namespace ibcs
{
    void Transformer :: transform(const shared_ptr<ASTNODE> &node){
        if (!node) return;
        
        // Debug: IN ra thong tin nut hien tai .. .
        cout <<"Transforming node :" << node -> type <<"|value :" << node -> value << endl;
        
        // Dispatch transformation logic based on node type..
        if (node -> type == "Assignment")
        {
            transformAssignment(node); 
        }
        else if (node -> type == "Expression")
        {
            transformExpression(node); 
        }
        else if (node -> type == "FunctionCall")
        {
            transformFunctionCall(node); 
        }
        else 
        {
            for (const auto &child : node -> children){
                transform(child); 
            }
        }
    }

    void Transformer :: transformAssignment(const shared_ptr<ASTNODE> &node)
    {
        if (node -> children.size() < 2) return; 
        string variable = node -> children[0] -> value; 
        string value = evaluateExpression(node -> children[1]); 

        semanticMap[variable] = value ; 
        cout <<"Assignment:" << variable << ":= " << variable << endl; 
    }

    void Transformer :: transformExpression(const shared_ptr<ASTNODE> &node)
    {
        string result = evaluateExpression(node);
        cout <<"Expression - Result = " << result << endl;
    }

    void Transformer :: transformFunctionCall(const shared_ptr<ASTNODE> &node)
    {
        string functionName = node -> value;
        vector<string> args; 

        for (size_t i = 0 ; i < node -> children.size(); ++i)
        {
            args.push_back(evaluateExpression(node -> children[i])); 
        }
        cout <<"Function Call - Calling : " << functionName << endl; 
        for (const auto &arg : args)
        {
            cout << arg << ","; 
        }
        cout <<")" << endl; 
    }

    string Transformer :: evaluateExpression(const shared_ptr<ASTNODE> &node)
    {
        if (!node) return ""; 
        if (node -> type == "Number" || node -> type == "String")
        {
            return node -> value; 
        }
        else if (node -> type == "Identifier")
        {
            if (semanticMap.find(node -> value) != semanticMap.end())
            {
                return semanticMap[node -> value]; 
            }
            else 
            {
                return "[Undefined]:" + node -> value +"]"; 
            }
            
        }
        else if (node -> type == "BinaryOP" and node -> children.size() ==2)
        {
            string left = evaluateExpression(node -> children[0]);
            string right = evaluateExpression(node -> children[1]);
            string op = node -> value; 
            
            return "(" + left + "" + op +"" + right +")";
        }
        return node -> value; 
    }

    // const unordered_map<string, string>& Transformer ::getSemanticMap() const
    // {
    //     return semanticMap; 
    // }
    unordered_map <string, string> semanticMap
    {}; 
}