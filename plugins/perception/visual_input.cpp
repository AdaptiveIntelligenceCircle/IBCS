#include <iostream> 
using namespace std; 
#include "plugins.h"
#include <atomic> 
#include <chrono>
#include <thread>

using namespace ibcs; 

class VisualInputPlugin final : public IPlugin
{
    public: 
    uint32_t width {512}, height{512}; 
    uint32_t fps{20}; 
    atomic<bool> running_{false}; 

    PluginContext *ctx_{nullptr}; 

    public: 
    const char *name() const override{
        return "perception.visual_input"; 
    }

    const char *id() const override 
    {
        return "id of routing.visual_input"; 
    }

    // Capability break ... // 
    virtual uint32_t capabitlities() const 
    {
        return static_cast<uint32_t>(Capability :: Kperception); 
    }

    // LifeCycle ....// 
    virtual bool init(PluginContext *ctx, const unordered_map<string,string> &cfg)
    {
        ctx_ = ctx; 
        if (auto it = cfg.find("width") ; it!= cfg.end())
        {
            width = stoul(it -> second); 
        }
        if (auto it = cfg.find("height") ; it != cfg.end())
        {
            height = stoul(it -> second); 
        }
        if (auto it = cfg.find("fps") ; it != cfg.end())
        {
            fps = stoul(it -> second);
        }  

        // TODO : activate Camera ..// 
        return true;
    }

    void start() override 
    {
        running_ = true; 
    }

    void pop() override
    {
        running_ = false; 
    }

    void onEvent() override{
        
    }

    void tick() override{
        if (running_ = false) return ; 
        
            // Generate a dummy gray frame, replace with camera grabber in production....// 
            Frame f; 
            f.width = width;
            f.height = height;
            f.channels = 3;  
        
        f.data.resize(width *height * 3, 127); 
        
        event e ; 
        e.type  = "Frame"; 
        e.source = name(); 
        e.meta = {{"Format", "RGB"}};
        e.payload.assign(reinterpret_cast<uint8_t*>(f.data.data()),
        reinterpret_cast<uint8_t *> (f.data.data()));

        ctx_-> emit(e) ; 

        this_thread :: sleep_for(chrono:: milliseconds(10));
    }
    
}; 

IBCS_PLUGIN_DESCRIPTOR ("perception.visual_input", "0.1.0", 
    static_cast<uint32_t> (Capability :: Kperception))

// IBCS_PLUGIN_FACTORY(VisualInputPlugin)


