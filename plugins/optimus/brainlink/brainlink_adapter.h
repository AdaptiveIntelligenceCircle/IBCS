#pragma once 
#include "interface.h"

#include <string>
#include <memory> 

#include <mutex> 
using namespace std;

namespace optimus :: brainlink
{
    // Concrete adapter : implements IBrainLink using project's transport..
    // it expects the transport header to be available at the path:

    // The transport header should provide simple,, 
    // Adapter is resilent; 
    class BrainLinkAdapter : public IBrainLink
    {
        public: 
        BrainLinkAdapter(); 
        ~BrainLinkAdapter(); 

        // IbrainLink .. 
        bool connect(const string &uri) override; 
        bool publish_telemetry(const Telemetry &t) override; 
        bool send_command(const HLCommand &c) override; 
        void on_command(CommandCallback cb) override; 
        void on_telemetry_ack(Telemetry cb) override; 
        bool health_check() override; 

        private: 
        // opaque transport pointer/handle.. 
        void *transport_handle_ = nullptr;
        string uri_; 
        CommandCallback cmd_cb_; 
        Telemetry telem_cb_;
        mutex cb_mtx_; 
        
        // internal helpers.. 
        bool ensure_transport(); 
        void transport_receive_loop(); 
        // optional background loop..
    }; 
} // namespace optimus : brainlink.. 