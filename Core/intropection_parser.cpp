#include "introspection_engine.h"

using namespace std; 

#include <iostream>
#include <string> 
#include <vector> 
#include <sstream>

#include <algorithm>
#include <cctype> 

namespace ibcs
{
    // helper : Chuan hoa y tuong (xoa khoang trong thua)
    string normalizeThought(const string &thought)
    {
        string result;
        bool capitalize = true;
        for (char ch : thought)
        {
            if (isspace(ch))
            {
                result += ' '; 
                capitalize = true; 
            }
            else{
                result += capitalize ? toupper(ch) : tolower(ch);
                capitalize = false; 
            }
        }
        static vector<string> blackList = {"uh", "hmm", "like", "just", "well"};
        for(const string&word : blackList)
        {
            size_t pos; 
            while((pos = result.find("" + word + "")) != string ::npos )
            {
                result.erase(pos, word.size()  + 2 );
            }
        }
    return result;
    }

    // phan tich  : lua lai cac suy nghi noi bo co chieu sau,..
    void IntrospectionEngine ::analyzeThoughtProcess(const vector<string> &ideastream)
    {
        innerThoughts.clear(); 
        for (const string &idea : ideastream)
        {
            string processed = normalizeThought(idea); 
            if (processed.length()> 15)
            {
                innerThoughts.push_back(processed);
            }
        }
        if (innerThoughts.empty())
        {
            innerThoughts.push_back("No meaningful"); 
        }
    }  

    // Tom tat theo logci dong suy nghi..chon ra vai noi dung
    string IntrospectionEngine :: summarizeIntropection()
    {
        if (innerThoughts.empty()) 
        return "no Thoughts processed";

        ostringstream summary; 
        summary <<"Introspective Summary start" ;
        int idx = 1 ; 

        for (const string &thought : innerThoughts)
        {
            summary << idx++ <<"." << thought << endl;
        }
        summary <<"End of Summary"; 
        return summary.str();
    }
}
