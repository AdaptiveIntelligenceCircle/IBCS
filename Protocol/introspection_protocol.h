#pragma once 
#include <iostream> 
#include <string> 

#include <vector> 
#include <functional> 
using namespace std;

namespace ibcs :: protocol
{
    struct IntrospectionRequest
    {
        string session_id; 
        string payload; 
        uint64_t timestamp; 
    }; 

    struct IntrospectionResponse
    {
        bool success; 
        string result; 
        string error; 
        uint64_t timestamp; 
    }; 

    class IntrospectionProtocol : public IntrospectionResponse
    {
        public: 
        virtual ~IntrospectionProtocol() = default; 

        // Parse incoming raw packet -> structured request..
        virtual IntrospectionProtocol parse(const string &blob); 

        // produce a response packet.. 
        virtual string encode_response(const IntrospectionProtocol &response);

        // Handle command (delegated to introspection engine.. )
        virtual IntrospectionProtocol handle(const IntrospectionRequest &request) = 0 ; 
        
    }; 
} // namespace ibcs :: protocol../ 