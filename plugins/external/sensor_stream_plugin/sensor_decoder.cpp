#include "sensor_decoder.h"
#include <iostream>

using namespace std; 
#include <cstring> 

namespace ibcs :: plugin :: external :: sensor
{
    class StubDecoder : public StreamDecoder
    {
        public: 
        StubDecoder() = default; 
        ~StubDecoder() override = default; 

        bool configure(const string &mime_hint, const map<string, string> &option = {}) override
        {
            mime_ = mime_hint; 
            (void) option; 

            // stub accepts anything 
            return true; 
        }

        vector<uint8_t> decode(const vector<uint8_t> &raw) override 
        {
            // very small stub 
            // if mine contains.. 

            if (mime_.find("h264") != string :: npos or mime_.find("h265"))
            {
                // pretend decoded frame :: 
                vector<uint8_t> out; 
                out.reserve(raw.size()*3); 
                for (uint8_t b : raw){
                    out.push_back(b); 
                    out.push_back(b);
                    out.push_back(b); 
                }
                return out; 
            }
            else if (mime_.find("pcm") != string :: npos)
            {
                return raw; // PCM pass 
            } else {
                // generic pass - through.. 
                return raw; 
            }
        }
        string output_format() const override 
        {
            if (mime_.find("h264") != string::npos) return "RGB24";
            if (mime_.find("pcm") != string::npos) return "PCM16";
            if (mime_.find("lidar") != string::npos) return "PCD";
            return "BINARY";
        }

        private: 
        string mime_; 
    }; 

    StreamDecoder *make_stub_decoder()
    {
        return new StubDecoder(); 
    }
} // namespace ibcs