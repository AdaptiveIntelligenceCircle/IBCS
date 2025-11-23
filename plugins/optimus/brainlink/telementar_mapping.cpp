#include "telementary_mapping.h"
using namespace std; 

#include <sstream> 
#include <iomanip> 

namespace optimus :: brainlink 
{
    static string escape(const string &s)
    {
        ostringstream o;

        for (char c : s)
        {
            switch(c)
            {
                case '\"': o << "\\\""; break;
                case '\\': o << "\\\\"; break;
                case '\n': o << "\\n"; break;
                default: o << c; break;
            }
        }
        return o.str(); 
    }

    string TelemetryMapping::to_payload(const Telemetry &t) 
    {
    ostringstream ss;
    ss << "{";
    ss << "\"id\":\"" << escape(t.id) << "\",";
    ss << "\"ts\":" << fixed << setprecision(3) << t.timestamp << ",";
    ss << "\"battery\":" << t.battery << ",";
    ss << "\"safety_blocked\":" << (t.safety_blocked ? "true" : "false") << ",";
    ss << "\"fault\":" << (t.fault ? "true" : "false") << ",";
    ss << "\"joints\":[";
    for (size_t i=0;i<t.joints.size();++i) 
    {
        ss << t.joints[i];
        if (i+1 < t.joints.size()) ss << ",";
    }
    ss << "],";
    ss << "\"velocities\":[";
    for (size_t i=0;i<t.velocities.size();++i) 
    {
        ss << t.velocities[i];
        if (i+1 < t.velocities.size()) ss << ",";
    }
    ss << "],";
    ss << "\"meta\":\"" << escape(t.meta_json) << "\"";
    ss << "}";
    return ss.str();
    }

    optional<Telemetry> TelemetryMapping::parse_ack(const string &payload) 
    {
    // naive parse: if payload contains "ack" or "received" return simple telemetry ack
    Telemetry t;
    t.id = "ack_auto";
    t.timestamp = 0.0;
    if (payload.find("ack") != string::npos || payload.find("received") != std::string::npos) {
        // mark success ack
        return t;
    }
    return nullopt;
    }
}