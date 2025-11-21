// ibcs/plugins/external/sensor_stream_plugin/sensor_stream_plugin.cpp
#include "sensor_plugin.h"
#include "sensor_decoder.h"
#include <iostream>
#include <chrono>
#include <sstream>

#include "base/plugin_interface.h"
#include "sensor_plugin.cpp"

namespace ibcs::plugin::external::sensor {

using Clock = std::chrono::steady_clock;

SensorStreamPlugin::SensorStreamPlugin() = default;
SensorStreamPlugin::~SensorStreamPlugin() {
    // stop all streams
    lock_guard<mutex> lk(streams_mtx_);
    for (auto &kv : streams_) {
        kv.second->running = false;
        if (kv.second->worker.joinable()) kv.second->worker.join();
    }
    streams_.clear();
}

ibcs::plugin::PluginInfo SensorStreamPlugin::info() const {
    return { "external.sensor_stream", "1.0.0", "IBCS External Team" };
}

bool SensorStreamPlugin::on_load() {
    cout << "[sensor_stream] plugin loaded\n";
    return true;
}

bool SensorStreamPlugin::on_unload() {
    cout << "[sensor_stream] plugin unloaded\n";
    // stop streams
    lock_guard<mutex> lk(streams_mtx_);
    for (auto &kv : streams_) {
        kv.second->running = false;
        if (kv.second->worker.joinable()) kv.second->worker.join();
    }
    streams_.clear();
    return true;
}

vector<string> SensorStreamPlugin::capabilities() const 
{
    return { "sensor.stream", "sensor.decode", "sensor.subscribe" };
}

bool SensorStreamPlugin::start_stream(const StreamConfig &cfg) {
    lock_guard<mutex> lk(streams_mtx_);
    if (streams_.count(cfg.source)) {
        cerr << "[sensor_stream] stream already exists: " << cfg.source << "\n";
        return false;
    }
    auto state = make_unique<StreamState>();
    state->cfg = cfg;
    state->max_queue = cfg.max_queue;
    state->decoder.reset(make_stub_decoder());
    // state->decoder->configure(cfg.mime_hint, cfg.extras);
    state->running = true;

    // worker thread processes queue and decodes if auto_decode
    state->worker = thread([this, s = state.get()]() {
        this->stream_worker_loop(s);
    });

    streams_.emplace(cfg.source, move(state));
    cout << "[sensor_stream] started stream: " << cfg.source << "\n";
    return true;
}

bool SensorStreamPlugin::stop_stream(const string &source) {
    lock_guard<mutex> lk(streams_mtx_);
    if (source.empty()) {
        // stop all
        for (auto &kv : streams_) {
            kv.second->running = false;
            if (kv.second->worker.joinable()) kv.second->worker.join();
        }
        streams_.clear();
        return true;
    }
    auto it = streams_.find(source);
    if (it == streams_.end()) return false;
    it->second->running = false;
    if (it->second->worker.joinable()) it->second->worker.join();
    streams_.erase(it);
    cout << "[sensor_stream] stopped stream: " << source << "\n";
    return true;
}

// string SensorStreamPlugin::subscribe(FrameCallback cb) {
//     lock_guard<mutex> lk(subs_mtx_);
//     std::string id = make_sub_id();
//     subscribers_.emplace(id, move(cb));
//     return id;
// }

// bool SensorStreamPlugin::unsubscribe(const string &subscriber_id) {
//     lock_guard<mutex> lk(subs_mtx_);
//     return subscribers_.erase(subscriber_id) > 0;
// }

bool SensorStreamPlugin::on_raw_packet(const RawPacket &pkt) {
    std::lock_guard<std::mutex> lk(streams_mtx_);
    auto it = streams_.find(pkt.meta.count("source") ? pkt.meta.at("source") : std::string());
    if (it == streams_.end()) {
        // fallback: try to find by matching source == pkt.mime or single stream
        if (streams_.size() == 1) it = streams_.begin();
        else return false;
    }
    auto &state = it->second;
    {
        std::unique_lock<std::mutex> qlk(state->mtx);
        if ((int)state->queue.size() >= state->max_queue) {
            // backpressure policy: drop oldest
            state->queue.pop();
        }
        state->queue.push(pkt);
        state->cv.notify_one();
    }
    return true;
}

void SensorStreamPlugin::stream_worker_loop(StreamState *state) {
    while (state->running) {
        RawPacket pkt;
        {
            std::unique_lock<std::mutex> qlk(state->mtx);
            if (state->queue.empty()) {
                state->cv.wait_for(qlk, std::chrono::milliseconds(200));
                if (!state->running) break;
                if (state->queue.empty()) continue;
            }
            pkt = state->queue.front();
            state->queue.pop();
        }

        Frame f;
        if (state->cfg.auto_decode && state->decoder) {
            auto out = state->decoder->decode(pkt.data);
            f.payload = std::move(out);
            // choose kind based on mime
            // if (pkt.mime_.find("h264") != std::string::npos || pkt.mime.find("video") != std::string::npos)
            //     f.kind = Frame::Kind::VIDEO;
            // else if (pkt.mime_.find("pcm") != std::string::npos || pkt.mime.find("audio") != std::string::npos)
            //     f.kind = Frame::Kind::AUDIO;
            // else if (pkt.mime_.find("lidar") != std::string::npos)
            //     f.kind = Frame::Kind::LIDAR;
            // else
            //     f.kind = Frame::Kind::TELEMETRY;

            // f.format = state->decoder->output_format();
            // f.timestamp_ms = pkt.timestamp_ms;
            // f.meta = pkt.meta;
        } else {
            // if not decoding, just wrap raw data as telemetry
            // f.kind = Frame::Kind::TELEMETRY;
            // f.payload = pkt.data;
            // f.format = pkt.mime;
            // f.timestamp_ms = pkt.timestamp_ms;
            // f.meta = pkt.meta;
        }

        // dispatch to subscribers
        dispatch_frame(f);
    }
}

void SensorStreamPlugin::dispatch_frame(const Frame &f) {
    lock_guard<mutex> lk(subs_mtx_);
    // for (auto &kv : subscribers_) {
    //     try {
    //         kv.second(f);
    //     } catch (...) {
    //         // swallow subscriber exceptions
    //     }
    // }
}

vector<string> SensorStreamPlugin::active_streams() const {
    vector<string> out;
    lock_guard<mutex> lk(streams_mtx_);
    for (auto &kv : streams_) out.push_back(kv.first);
    return out;
}

string SensorStreamPlugin::make_sub_id() {
    ostringstream ss;
    // ss << "sub_" << (next_sub_id_++);
    return ss.str();
}

// loader symbols
extern "C" ibcs::plugin::IPlugin* create_plugin() {
    // return new SensorStreamPlugin();
}
extern "C" void destroy_plugin(ibcs::plugin::IPlugin* p) {
    delete p;
}

} // namespace ibcs::plugin::external::sensor
