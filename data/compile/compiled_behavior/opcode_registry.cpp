#include "opcode_registry.h"

namespace ibcs :: compile{
    int OpcodeRegistry :: resolve_id(const CompileContext &ctx, const string &name)
    {
        auto *op = ctx.get_opcode(name); 
        return op ? op -> id : -1; 
    }
}