// ibcs/plugins/external/perception/perception_plugin.cpp
#include "perception_plugin.h"
#include "perception_model_loader.h"
#include <iostream>
#include <chrono>
#include <sstream>

namespace ibcs::plugin::external::perception {

PerceptionPlugin::PerceptionPlugin()
    : loader_(std::make_unique<ModelLoader>())
{
}

PerceptionPlugin::~PerceptionPlugin() {
    // ensure all models unloaded
    for (auto &kv : models_) {
        loader_->unload(kv.first);
    }
}

ibcs::plugin::PluginInfo PerceptionPlugin::info() const {
    return { "external.perception", "1.0.0", "IBCS External Team" };
}

bool PerceptionPlugin::on_load() {
    std::cout << "[perception] plugin loaded\n";
    // optionally load default embedded model (stub)
    default_model_id_ = "default-stub-model";
    bool ok = loader_->load("builtin://default-stub", default_model_id_);
    if (ok) models_.emplace(default_model_id_, "builtin://default-stub");
    return true;
}

bool PerceptionPlugin::on_unload() {
    std::cout << "[perception] plugin unloaded\n";
    // unload loaded models
    for (auto &kv : models_) {
        loader_->unload(kv.first);
    }
    models_.clear();
    default_model_id_.clear();
    return true;
}

std::vector<std::string> PerceptionPlugin::capabilities() const {
    return { "perception.run", "perception.load_model", "perception.unload_model" };
}

bool PerceptionPlugin::load_model(const std::string &model_path, const std::string &model_id) {
    std::string id = model_id.empty() ? model_path : model_id;
    if (id.empty()) return false;
    bool ok = loader_->load(model_path, id);
    if (ok) models_.emplace(id, model_path);
    return ok;
}

bool PerceptionPlugin::unload_model(const std::string &model_id) {
    if (model_id.empty()) return false;
    auto it = models_.find(model_id);
    if (it == models_.end()) return false;
    bool ok = loader_->unload(model_id);
    if (ok) models_.erase(it);
    return ok;
}

std::vector<std::string> PerceptionPlugin::list_models() const {
    std::vector<std::string> out;
    out.reserve(models_.size());
    for (auto &kv : models_) out.push_back(kv.first);
    return out;
}

std::string PerceptionPlugin::resolve_model(const std::string &model_id) const {
    if (!model_id.empty()) {
        if (models_.count(model_id)) return model_id;
        // not found -> return empty
        return {};
    }
    // return default if exists
    if (!default_model_id_.empty() && models_.count(default_model_id_)) return default_model_id_;
    if (!models_.empty()) return models_.begin()->first;
    return {};
}

PerceptionResult PerceptionPlugin::run_inference(const PerceptionInput &in, const std::string &model_id) {
    PerceptionResult res;
    std::string mid = resolve_model(model_id);
    if (mid.empty()) {
        res.success = false;
        res.error = "no_model_loaded";
        return res;
    }

    // Simple resource/time accounting stub
    auto start = std::chrono::steady_clock::now();
    std::string raw = loader_->infer(mid, in.image_data);
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start).count();

    // parse the stub JSON-ish result (we won't include full JSON parser here; do naive parse)
    // Expecting {"model":"id","detections":[{...},...]}
    res.success = true;
    res.model_used = mid;

    // naive parse: find occurrences of "label": and "confidence":
    size_t pos = 0;
    while (true) {
        pos = raw.find("\"label\":\"", pos);
        if (pos == std::string::npos) break;
        pos += 9;
        size_t end = raw.find("\"", pos);
        if (end == std::string::npos) break;
        std::string label = raw.substr(pos, end - pos);
        // find confidence
        size_t cpos = raw.find("\"confidence\":", end);
        float conf = 0.0f;
        if (cpos != std::string::npos) {
            cpos += 13;
            size_t cend = raw.find_first_of(",}]", cpos);
            if (cend != std::string::npos) {
                std::string cs = raw.substr(cpos, cend - cpos);
                try { conf = std::stof(cs); } catch(...) { conf = 0.0f; }
            }
        }
        // bbox
        float bx=0, by=0, bw=0, bh=0;
        size_t bpos = raw.find("\"bbox\":", end);
        if (bpos != std::string::npos) {
            bpos = raw.find("[", bpos);
            if (bpos != std::string::npos) {
                bpos++;
                size_t bsep = raw.find(",", bpos);
                if (bsep != std::string::npos) {
                    try { bx = std::stof(raw.substr(bpos, bsep-bpos)); } catch(...) {}
                    size_t bpos2 = bsep+1;
                    size_t bsep2 = raw.find(",", bpos2);
                    if (bsep2 != std::string::npos) {
                        try { by = std::stof(raw.substr(bpos2, bsep2-bpos2)); } catch(...) {}
                        size_t bpos3 = bsep2+1;
                        size_t bsep3 = raw.find(",", bpos3);
                        if (bsep3 != std::string::npos) {
                            try { bw = std::stof(raw.substr(bpos3, bsep3-bpos3)); } catch(...) {}
                            size_t bpos4 = bsep3+1;
                            size_t bend = raw.find("]", bpos4);
                            if (bend != std::string::npos) {
                                try { bh = std::stof(raw.substr(bpos4, bend-bpos4)); } catch(...) {}
                            }
                        }
                    }
                }
            }
        }
        Detection d;
        d.label = label;
        d.confidence = conf;
        d.x = bx; d.y = by; d.w = bw; d.h = bh;
        res.detections.push_back(d);
        pos = end;
    }

    // attach a short performance note in error field when ok
    std::ostringstream note;
    note << "inference_ms=" << dur;
    res.error = note.str();

    return res;
}

// Exported symbols for native loader
extern "C" ibcs::plugin::IPlugin* create_plugin() {
    return new PerceptionPlugin();
}

extern "C" void destroy_plugin(ibcs::plugin::IPlugin* p) {
    delete p;
}

} // namespace ibcs::plugin::external::perception
