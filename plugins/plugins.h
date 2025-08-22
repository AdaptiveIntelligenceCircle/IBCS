#pragma once 
#include <cstdint>
#include <iostream>

#include <vector> 
#include <memory> 
#include <string> 

#include <unordered_map>

using namespace std; 
namespace ibcs 
{
    // -- Versioning ---// 
    static constexpr int PLUGIN_API_VERSION_MAJOR = 1; 
    static constexpr int PLUGIN_API_VERSION_MINOR = 0; 

    // -- Basic types --- // 
    using bytes = vector<uint8_t>; 

    struct KV
    {
        string key; 
        string value; 
    };

    struct event
    {
        string type ; 
        string source; 
        unordered_map<string, string> meta; 
        // Bytes payload;  
    }; 

    struct Frame
    {
        uint32_t width{0}, height{0}, channels{0}; 
        vector<uint8_t> data ; 
    }; 

    struct AudioBuffer
    {
        uint32_t sample_rate{16000}; 
        uint16_t channels{1}; 
        vector<float> samples; 
    }; 

    // -- Context injected by runtime --- // 
    struct PluginContext
    {
        // thread - safe emit back to runtime .. // 
        virtual void emit() = 0;

        // synchronous query to runtime services
        virtual string call(const string &service, const string request_json) = 0; 

        virtual ~PluginContext() = default; 
    };  

    // --- Capability declaration... --- // 
    enum class Capability : uint32_t // 16 bit cung duoc .. 
    {
        kNone  = 0 , 
        Kperception = 1 << 0, 
        kExplain = 1 << 1 , 
        kNeuroBridge = 1 << 2, 
        kActuation = 1 << 3
    };

    inline Capability operator |(Capability a , Capability b)
    {
        return static_cast<Capability>(static_cast<uint32_t>(a) || static_cast<uint32_t> (b)); 
    }

    // -- Base Plugin interface .. /--- 
    class IPlugin
    {
        public: 
        virtual ~IPlugin() = default;
        
        // Name/id for routing and metrics ... 
        virtual const char *name() const = 0; 
        virtual const char *id () const = 0 ;
        
        // Capability break..// 
        virtual uint32_t capabilities() const =0; 

        // Lifecycle .. // 
        virtual bool init(PluginContext *ctx, const unordered_map<string, string> &config) = 0; 

        virtual void start() = 0; 
        virtual void pop() = 0; 

        // messaging passing (non - blocking ; plugin may spawn worker threads )
        virtual void onEvent()  =0 ; 

        // optional tick for pplling sources (camera, device , etc,...)
        virtual void tick(){}
    }; 

    // C-- api symbols for dlopen/dlsym stability .. / / 

    extern "C"
    {
        // required : factory -- /// 
        typedef ibcs::IPlugin* (*CreatePluginFn)();
        typedef void (*DestroyPluginFn)(ibcs::IPlugin*);

// Required: descriptor for quick filtering before instantiation
        struct PluginDescriptor 
        {
            const char* name;
            const char* version;
            uint32_t capabilities; // same bitmask as IPlugin::capabilities
            int api_version_major;
            int api_version_minor;
        };

    typedef PluginDescriptor (*GetDescriptorFn)();

    }
}

#if defined(_WIN32)
  #define IBCS_EXPORT extern "C" __declspec(dllexport)
#else
  #define IBCS_EXPORT extern "C"
#endif

#define IBCS_PLUGIN_DESCRIPTOR(NM, VER, CAPS) \
  IBCS_EXPORT ibcs::PluginDescriptor ibcs_get_descriptor() { \
    return { NM, VER, CAPS, ibcs::PLUGIN_API_VERSION_MAJOR, ibcs::PLUGIN_API_VERSION_MINOR }; \
  }

#define IBCS_PLUGIN_FACTORY(Cls) \
  IBCS_EXPORT ibcs::IPlugin* ibcs_create_plugin() { return new Cls(); } \
  IBCS_EXPORT void ibcs_destroy_plugin(ibcs::IPlugin* p) { delete p; }
