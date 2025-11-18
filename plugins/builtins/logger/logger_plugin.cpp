// ibcs/plugins/builtins/logger/logger_plugin.cpp
#include "logger_plugin.h"
#include <iostream>
#include <chrono>
#include <ctime>
using namespace std; 

namespace ibcs::plugin::builtins 
{

LoggerPlugin::LoggerPlugin() 
{

}

LoggerPlugin::~LoggerPlugin() 
{

}

PluginInfo LoggerPlugin::info() const 
{
    return 
    {
        "builtins.logger",
        "1.0.0",
        "IBCS Core Team"
    };
}

bool LoggerPlugin::on_load() 
{
    logFile_.open("ibcs_log.txt", ios::app);
    if (!logFile_) 
    {
        cerr << "[logger] failed to open log file\n";
        return false;
    }

    cout << "[logger] plugin loaded\n";
    return true;
}

bool LoggerPlugin::on_unload() 
{
    cout << "[logger] plugin unloaded\n";
    if (logFile_.is_open()) logFile_.close();
    return true;
}

vector<string> LoggerPlugin::capabilities() const 
{
    return 
    {
        "log",
        "trace_emit",
        "diagnostics"
    };
}

void LoggerPlugin::log(const string &level, const string &msg) 
{
    if (!logFile_) return;

    // Timestamp
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);

    logFile_ << "[" <<string(ctime(&t)).substr(0,24) << "] "
             << "[" << level << "] "
             << msg << "\n";

    logFile_.flush();
}

} // namespace ibcs::plugin::builtins

// ---------- Dynamic Loading Entry Points -----------

extern "C" ibcs::plugin::IPlugin* create_plugin() 
{
    return new ibcs::plugin::builtins::LoggerPlugin();
}

extern "C" void destroy_plugin(ibcs::plugin::IPlugin* p) 
{
    delete p;
}
