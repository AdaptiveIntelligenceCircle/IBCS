#include "behavior_model.h"
using namespace std; 

#include <string> 
#include <algorithm>

#include <vector> 
#include <sstream>

namespace ibcs
{
    void BehaviorModel :: addNode(const BehaviorNode &node)
    {
        behaviorTree.push_back(node); 
        changeLog.push_back("Added Node:" + node.id); 
    }

    bool BehaviorModel :: removeNodebyId(const string &id)
    {
        for (auto it = behaviorTree.begin(); it!= behaviorTree.end(); it++)
        {
            if (it -> id == id)
            {
                changeLog.push_back("Remove Node:" + id);
                behaviorTree.erase(it); 
                return true; 
            }
        }
        return false; 
    }

    bool BehaviorModel :: updateNode(const string &id, const BehaviorNode &behavior)
    {
        for (auto &node : behaviorTree)
        {
            if (node.id == id)
            {
                node.trigger = behavior.trigger; 
                node.condition = behavior.condition; 
                node.action = behavior.action; 

                changeLog.push_back("Update Node :" + id);
                return true; 
            }
        }
        return false; 
    }

    optional<BehaviorNode> BehaviorModel :: FindByTrigger(const string &trigger)
    {
        for (const auto &node : behaviorTree)
        {
            if (node.trigger == trigger)
            return node; 
        }
        return nullopt; 
    }

    optional<BehaviorNode> BehaviorModel :: FindByCondition(const string &conditional)
    {
        for (const auto &node : behaviorTree)
        {
           if (node.condition == conditional)
           return node; 
        }
        return nullopt; 
    }
    
    const vector<BehaviorNode> &BehaviorModel :: getNodes() const 
    {
        return behaviorTree; 
    }

    void BehaviorModel :: clearNodes()
    {
        changeLog.push_back("Cleared all nodes"); 
        behaviorTree.clear(); 
    }

    void BehaviorModel :: logChanges() const 
    {
        cout <<"BehaviorModel changes log:\n";
        for (const auto &log : changeLog)
        {
            cout <<"-" << log << endl; 
        }
    }
}