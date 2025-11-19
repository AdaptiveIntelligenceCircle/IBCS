#pragma once 
#include <string> 

#include <vector> 
#include <cstdint> 

using namespace std; 

namespace ibcs :: plugin :: external :: perception
{
    class ModelLoader
    {
        public: 
        ModelLoader() = default; 
        virtual ~ModelLoader() = default; 

        // load model file and reuturn a model identifire..
        virtual bool load(const string &path, const string &mdoel_it = ""); 

        // unload modle by id .. 
        virtual bool unload(const string &path, const string &model_id = ""); 

        // run inference .. 
        virtual string infer(const string &model_id, const vector<uint8_t> &image_data);
        
        // query if models exists.. 
        virtual bool exists(const string &model_id) const; 
        
    };
} // namespace ibcs :: plugin :: external :: perception.. /  

