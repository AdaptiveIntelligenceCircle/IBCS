#pragma once 

#include "interface.h"
#include <string>

using namespace std; 

namespace optimus :: brainlink
{
    // convert Telemetry struct to Json payload (string).. 
    class TelemetryMapping
    {
        public: 
        static string to_payload(const Telemetry &t); 

        // parse incoming ack payload -> Telemetry (optional)..
        static optional<Telemetry> parse_ack(const string &payload); 
    }; 
} // namespace optimus : brainlink.. 