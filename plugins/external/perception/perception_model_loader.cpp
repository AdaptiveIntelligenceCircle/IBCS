#include "perception_model_loader.h"
using namespace std; 
#include <unordered_set>

#include <iostream> 
#include <sstream> 

#include <string> 

namespace ibcs :: plugin :: external :: perception
{
    static unordered_set<string> g_loaded_models; 

    bool ModelLoader :: load(const string &path, const string &model_id)
    {
        string id = model_id.empty() ? path : model_id; 
        if(id.empty()) return false; 

        // simulate loading .. 
        cout <<"ModelLoader : stub loading model : " << path << endl; 
        g_loaded_models.insert(id);
        return true; 
    }

    bool ModelLoader :: unload(const string &path, const string &model_id = "")
    {
        if (model_id.empty()) return false; 
        auto it = g_loaded_models.find(model_id); 
        if (it == g_loaded_models.end()) return false; 
        g_loaded_models.erase(it); 
        cout << "Model Loader : unloaded model :" << model_id << endl; 
        return true; 
    }

    string ModelLoader :: infer(const string &model_id, const vector<uint8_t> &image_data)
    {
        ostringstream oss; 

        if (!exists(model_id)){
            oss <<"Error : model not loaded" << endl; 
            return oss.str(); 
        }

        // simple : detect either "object - A" if.. 
        size_t s = image_data.size(); 
        if (s == 0)
        {
            oss <<"Detections" << endl; 
            return oss.str(); 
        }

        oss << "{"; 
        oss << "Model :" << model_id << "\",";
        oss << "\"Detections\": [";

        // produce up to 2 stub detections.. //
        int n = (int) ((s % 3) + 1); 
        for (int i = 0 ; i < n;  ++i)
        {
            string label = (i % 2 == 0) ? "object - A" : "object - B";
            float conf = 0.6f + 0.1f *(i%3);
            oss << "{";
            oss << "\"label\":\"" << label << "\",";
            oss << "\"confidence\":" << conf << ",";
            oss << "\"bbox\":[" << 0.1*i << "," << 0.1*i << "," << 0.2 << "," << 0.2 << "]";
            oss << "}";   

            if (i + 1 < n) oss <<","; 
        }

        oss << "]"; 
        oss << "}"; 
        return oss.str(); 
    }

    bool ModelLoader :: exists(const string &model_id) const 
    {
        if (model_id.empty()) return false; 
        return g_loaded_models.find(model_id) != g_loaded_models.end(); 
    }
} // namespace ibcs :: plugin :: external :: perception.. // 