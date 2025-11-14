#pragma once 
#include <iostream>
using namespace std; 

#include <algorithm> 
#include <string> 
#include <vector> 

namespace ibcs
{
    struct BehaviorNode
    {
        string trigger; 
        string condition;
        string action ;
        string id; 
    }; 
    
    class BehaviorModel
    {
        public: 
        void addNode(const BehaviorNode &node); 
        bool removeNodebyId(const string &id); 
        bool updateNode(const string &id, const BehaviorNode &behavior); 

        const vector<BehaviorNode> &getNodes()const; 
        optional<BehaviorNode> FindByTrigger(const string &trigger); 
        optional<BehaviorNode> FindByCondition(const string &condition); 

        const vector <BehaviorNode> &getNodes() const; 
        void clearNodes(); 
        void logChanges() const; 

        private:
        vector<BehaviorNode> behaviorTree;
        vector<string> changeLog; 
    }; 
}
