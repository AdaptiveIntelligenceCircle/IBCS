#pragma once 
#include <string> 

#include <vector>
#include <cstdint> 

#include <map> 
using namespace std; 

namespace ibcs :: plugin :: external :: sensor
{
    class StreamDecoder
    {
        public: 
        StreamDecoder() = default; 
        virtual ~StreamDecoder() = default;

        // configure decoder with MIME hint or model.. 
        virtual bool configure(const string &mine_hint, const map<string, string> &options = {}) = 0;

        // Feed raw packet to attempt to decode.. 
        // For simplicity, we return one Frame Like --- 
        virtual vector<uint8_t> decode(const vector<uint8_t> &raw) = 0; 

        // Query expected output format string 
        virtual string output_format() const = 0; 
    }; 

    StreamDecoder *make_stub_decoder(); 
} // namespace ibcs :: plugin :: external :: sensor .. 