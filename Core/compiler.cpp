#include "compiler.h"
namespace ibcs
{
    Compiler :: Compiler(){}

    string Compiler :: compileBehavior(const BehaviorModel &model , const ContextManager &context)
    {
        string code; 
        for (const auto &node : model.getNodes())
        {
            code += "if(" + node.condition +")" + node.action;
        }
        optimizeCompiledCode(code);
        return code; 
    }

    void Compiler :: optimizeCompiledCode(string &code){
        // Toi uu ma nguon o muc thap (loai bo du thua logic)
    }
}
