#include "interpreter.h"
using namespace std; 

#include <string> 
#include <algorithm> 

#include <sstream> 

namespace ibcs
{
    void Interpreter :: execute(const vector<string> &instructions)
    {
        for (const auto &instr : instructions)
        {
            interpret(instr); 
        }
    }

    void Interpreter :: interpret(const string &instruction)
    {
        if (instruction == "hello_world")
        {
            cout <<"Interpreter - Hello world" << endl; 
        }
        else if (instruction.rfind("Print:", 0) == 0)
        {
            string message = instruction.substr(6);
            cout <<"Interpreter" << message << endl; 
        }
        else if(instruction.rfind("set :", 0) == 0)
        {
            auto[key, value] = parsekeyValues(instruction.substr(4)); 
            memory[key] = value; 
            cout <<"[Interpreter] Set [" << key << "] = " << value << endl;
        }
        else if (instruction.rfind("get:", 0) == 0)
        {
            string key = instruction.substr(4);
            if (memory.find(key) != memory.end())
            {
                cout << "[Interpreter] Get [" << key <<"] = " << memory[key] <<endl; 
            }
            else 
            {
                cout <<" Get " << key <<"not found." << endl;  
            }
        }
        else if (instruction == "Clear_memory")
        {
            memory.clear(); 
            cout <<"Memory Clear" << endl; 
        }
        else 
        {
            cout <<"[Interpreter] Unknown" << endl; 
        }
    }
    
    pair <string, string> Interpreter :: parsekeyValues(const string &input)
    {
        size_t pos =  input.find("="); 
        if (pos == string :: npos)
        {
            return {
                "invalid", "invalid"
            };
            string key = input.substr(0, pos); 
            string value = input.substr(pos + 1); 
            return {
                key, value
            } ;
        }
    }
}
