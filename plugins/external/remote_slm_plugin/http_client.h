#pragma once 
#include <string> 
#include <map> 

using namespace std; 

namespace ibcs :: plugin :: external :: remote_slm
{
    class HttpClient
    {
        HttpClient(); 
        ~HttpClient(); 

        // set timeout in seconds.. 
        void set_timeout(int seconds); 

        // perform a post to url with headers and body.. 
        pair<bool, string> post(const string &url, 
        const map<string, string> &headers, const string &body);

        // perform a GET to url with headers...
        pair<bool, string> get(const string &url, const map<string, string> &headers); 

        private:
        int timeout_seconds_ ; 
    }; 
} // namespace ibcs :: 