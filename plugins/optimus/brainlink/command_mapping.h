#pragma once 
#include "interface.h"

#include <string> 
using namespace std; 

namespace optimus :: brainlink
{
    // Convert a Json-like or transport payload into HLCommand.. 
    // For now we provide a small parser that experts a simple Json
    // You can replace with a robust Json parser (later..)

    class CommandMapping
    {
        public: 
        // parse raw payload (string) into HLCommand.. 
        // returns optional empty if parse fails..
        static optional<HLCommand> parse_from_payload(const string &payload);
        
        //Convert HLcommand to a compact Json string suitable for sending over transport.. 
        static string to_payload(const HLCommand &cmd); 
    }; 
} // namespace optimus :: brainlink.. 