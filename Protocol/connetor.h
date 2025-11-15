#pragma once 
#include <string> 

#include <functional> 
#include <memory> 

using namespace std; 

namespace ibcs :: protocol
{
    class IntrospectionProtocol; 

    class Connector
    {
        public: 
        virtual ~Connector() = default; 

        // send raw string to remote node.. 
        virtual bool send(const string &blob) = 0 ; 

        // Register receive callback..
        virtual void on_receive(function<void(const string &) >rec);
        
        // Bind protocol.. 
        void attach_protocol(shared_ptr<IntrospectionProtocol> protocol) ; 

        protected: 
        shared_ptr<IntrospectionProtocol> protocol_; 
    }; 

    unique_ptr<Connector> make_tcp_connector(const string &address); 
    unique_ptr<Connector> make_local_connector(); 
} // namespace ibcs :: protocol.. 
