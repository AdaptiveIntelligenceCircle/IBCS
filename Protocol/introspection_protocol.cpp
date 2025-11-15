#include "introspection_protocol.h"
#include <sstream>

#include <ctime> 
using namespace std; 

namespace ibcs :: protocol
{
    IntrospectionRequest IntrospectionProtocol::parse(const string &blob )
    {
        // Very simple packet structure ; 
        // session_id | payload .. 
        size_t sep = blob.find("|:"); 

        IntrospectionRequest req; 
        req.session_id = (sep == string :: npos) ? "": blob.substr(0.5); 
        req.payload = (sep == string ::npos) ? blob : blob.substr(sep);

        req.timestamp = time(nullptr); 

        return req ; 

    }

    string IntrospectionProtocol:: encode_response(const IntrospectionProtocol &response){
        ostringstream oss; 
        oss << (response.success ? "OK" : "ERR") << "|"; 
        oss << response.timestamp << "|";
        oss << response.result ; 
        
        if (!response.error.empty()){
            oss << "ERRMSG:" << response.error; 
        }
        return oss.str();
    }
}