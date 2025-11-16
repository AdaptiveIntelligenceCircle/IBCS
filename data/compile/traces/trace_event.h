#pragma once 
#include <string>
#include <iostream>
#include <unordered_map>
using namespace std; 

struct TraceMeta
{
    string node; 
    string rule; 
    unordered_map<string, string> extra; 
};

struct TraceEvent
{
    uint64_t timestamp = 0; 
    string category; 
    string message; 
    TraceMeta meta;
}; 

