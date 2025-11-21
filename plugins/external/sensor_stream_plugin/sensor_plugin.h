#pragma once 
#include <functional> 

#include <string> 
#include <vector>

#include <algorithm> 
#include <cstddef> 

#include <cmath> 
using namespace std; 

#include <thread> 
#include <atomic> 

#include <mutex> 
#include <condition_variable>

#include <mutex> 
#include <optional>

#include <map> 
#include <cstdint>
#include <queue>

namespace ibcs :: plugin :: external :: sensor 
{
    // Raw packet from stream (transport specific)

    struct RawPacket
    {
        vector<uint8_t> data; 
        string mine; 
        uint64_t timestamp_ms  = 0 ; 
        map <string, string> meta; 
    }; 

    // Decoedd frame (generic) 

    struct Frame
    {
        enum class Kind 
        {
            VIDEO, AUDIO, LIDAR, TELEMETRY
        }kind; 

        vector<uint8_t> payload; 
        string format; 
        uint64_t timepstamp_ms = 0; 
        map<string, string> meta; 
    };

    // subcriber callback ...
    using FrameCallback = function<void(const Frame &frame)>; 

    // Config for stream .. 
    struct StreamConfig
    {
        string source; 
        string mine_hint; 
        int max_queue = 0;
        bool auto_decode = true; 
        map<string, string> extras; 
    }; 

    class StreamDecoder;

    class SensorStreamPlugin 
    {
        public: 
        SensorStreamPlugin(); 
        ~SensorStreamPlugin();  

        // IPlugin ;
        //ibcs :: plugin :: PluginInfo info() const override;
        bool on_load() ; 
        bool on_unload();  

        vector<string> capabilities() const; 

        // Stream control API.. 
        bool start_stream(const StreamConfig &cfg); // config .. 
        bool stop_stream(const string &source = ""); 

        // subcribe to decoded frames .. 
        string subscribe(FrameCallback cb);
        bool unsubcriber(const string &subcriber_id);

        // push raw packet (for intergration).. 
        bool on_raw_packet(const RawPacket &pkt); 

        // list active streams .. 
        vector<string> active_streams() const; 

        private: 
        struct StreamState
        {
            StreamConfig cfg ; 
            unique_ptr<StreamDecoder> decoder; 
            thread worker;
            atomic<bool> running{false}; 

            mutex mtx; 
            condition_variable cv;
            queue<RawPacket> queue; 
            int max_queue = 0; 
        }; 

        // Mapping source -> stream state.. 
        map<string, unique_ptr<StreamState>> streams_; 
        mutable mutex streams_mtx_; 

        // subcribers .. 
        map<string, FrameCallback> subcribers_;
        mutable mutex subs_mtx_;
        int next_sub_id = 1; 
        
        // helpers 
        void stream_worker_loop(StreamState *state); 
        void dispatch_frame(const Frame &f); 
        string make_sub_id(); 
    }; 
    extern "C"
    {
        // ibcs :: plugin :: IPlugin *create_plugin(); 
        void destroy_plugin(SensorStreamPlugin *plugin);
    }
}