// optimus/brainlink/brainlink_adapter.cpp
#include "brainlink_adapter.h"
#include "command_mapping.h"
// #include "telemetry_mapping.h"
#include <iostream>
#include <thread>
#include <chrono>

//
// NOTE: we include your project's transport header here. The file path was uploaded and is:
//   /mnt/data/transport.h
//
// The build system in your environment should map that path correctly.
// If your transport header exposes different API names, adapt the calls below.
//

namespace optimus::brainlink {

BrainLinkAdapter::BrainLinkAdapter() : transport_handle_(nullptr) {}
BrainLinkAdapter::~BrainLinkAdapter() {
    // TODO: gracefully stop receive loop if implemented
}

bool BrainLinkAdapter::ensure_transport() {
    if (transport_handle_) return true;
    // attempt to init transport based on provided header's API
    // This code uses a hypothetical minimal transport API:
    //  - transport_create(uri) -> void*
    //  - transport_send(void* h, const char* topic, const char* payload)
    //  - transport_subscribe(void* h, const char* topic, void(*cb)(const char* payload, void* ctx), void* ctx)
    //  - transport_ping(void* h) -> bool
    //
    // If your /mnt/data/transport.h uses different names, replace accordingly.

#ifdef TRANSPORT_CREATE
    transport_handle_ = (void*)TRANSPORT_CREATE(uri_.c_str());
    if (!transport_handle_) {
        std::cerr << "[BrainLinkAdapter] transport_create failed\n";
        return false;
    }
    return true;
#else
    // Fallback: try to call functions by name if they exist via C API
    // We use preprocessor guards to avoid compile errors if transport.h not providing API.
    // If transport.h doesn't provide expected symbols, we fallback to false.
    (void)transport_handle_;
    (void)uri_;
    cerr << "[BrainLinkAdapter] No transport factory macro TRANSPORT_CREATE — adapt transport bindings in code.\n";
    return false;
#endif
}

bool BrainLinkAdapter::connect(const string &uri) {
    uri_ = uri;
    bool ok = ensure_transport();
    if (!ok) return false;

    // attempt a subscribe to "ibcs/commands/optimus"
#ifdef TRANSPORT_SUBSCRIBE
    auto lambda = [](const char* payload, void* ctx) {
        BrainLinkAdapter* self = reinterpret_cast<BrainLinkAdapter*>(ctx);
        if (!self) return;
        std::string p(payload ? payload : "");
        auto ocmd = CommandMapping::parse_from_payload(p);
        if (ocmd) {
            std::lock_guard<std::mutex> lk(self->cb_mtx_);
            if (self->cmd_cb_) self->cmd_cb_(*ocmd);
        }
    };
    TRANSPORT_SUBSCRIBE(transport_handle_, "ibcs/commands/optimus", lambda, (void*)this);
    return true;
#else
    (void)uri;
    cerr << "[BrainLinkAdapter] subscribe macro TRANSPORT_SUBSCRIBE not defined — please bind to your transport API.\n";
    return false;
#endif
}

bool BrainLinkAdapter::publish_telemetry(const Telemetry &t) {
    // Serialize telemetry -> payload
    // string payload = Telemetry::to_payload(t);
#ifdef TRANSPORT_SEND
    if (!ensure_transport()) return false;
    TRANSPORT_SEND(transport_handle_, "ibcs/telemetry/optimus", payload.c_str());
    return true;
#else
    // (void)payload;
    // cerr << "[BrainLinkAdapter] TRANSPORT_SEND not defined — cannot send telemetry. Payload preview:\n" << payload << "\n";
    return false;
#endif
}

bool BrainLinkAdapter::send_command(const HLCommand &c) {
    string payload = CommandMapping::to_payload(c);
#ifdef TRANSPORT_SEND
    if (!ensure_transport()) return false;
    TRANSPORT_SEND(transport_handle_, "optimus/commands/local", payload.c_str());
    return true;
#else
    (void)payload;
    std::cerr << "[BrainLinkAdapter] TRANSPORT_SEND not defined — cannot send command. Payload preview:\n" << payload << "\n";
    return false;
#endif
}

void BrainLinkAdapter::on_command(CommandCallback cb) {
    lock_guard<mutex> lk(cb_mtx_);
    cmd_cb_ = std::move(cb);
}

void BrainLinkAdapter::on_telemetry_ack(Telemetry cb) {
    lock_guard<mutex> lk(cb_mtx_);
    telem_cb_ = move(cb);
}

bool BrainLinkAdapter::health_check() {
#ifdef TRANSPORT_PING
    if (!ensure_transport()) return false;
    return TRANSPORT_PING(transport_handle_);
#else
    cerr << "[BrainLinkAdapter] TRANSPORT_PING not defined. Defaulting to false.\n";
    return false;
#endif
}

} // namespace optimus::brainlink
