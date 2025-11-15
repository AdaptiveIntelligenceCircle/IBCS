#include "connetor.h"
#include "introspection_protocol.h"

using namespace std; 
#include <iostream>

namespace ibcs :: protocol
{
    void Connector :: attach_protocol(shared_ptr<IntrospectionProtocol> protocol)
    {
        protocol_ = protocol ; 
    }

    class LocalConnector : public Connector 
    {
        private: 
        function<void(const string &)> on_recv; 

        public: 
        bool send(const string &blob) override{
            if (on_recv)
            {
                on_recv(blob); 
                return true; 
            }
            return false; 
        }

        void on_receive(function<void(const string &)> cb) override{
            on_recv = cb; 
        }
    }; 
    unique_ptr<Connector> make_local_connnector()
    {
        return make_unique<LocalConnector>(); 
    }

    class TCPConnector : public Connector{
        string addr_; 
        int port_; 
        function<void(const string &)> recv_cb; 

        public:
        TCPConnector (const string &addr , int port)
        : addr_(addr), port_(port){}

        bool send(const string &blob) override{
            cout << "TCP Connector - Sending to" << addr_ << "" << endl; 
            return true ;
        }

        void on_receive(function<void(const string &) > cb) override 
        {
            recv_cb = cb;
        }

        unique_ptr<Connector> make_tcp_connector(const string &address, int port)
        {
            return make_unique<TCPConnector> (address, port); 
        }
    }; 
}