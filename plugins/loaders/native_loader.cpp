// ibcs/plugins/loaders/native_loader.cpp
#include "native_loader.h"
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace ibcs::plugin::native 
{

using namespace ibcs::plugin;

typedef PluginPtr (*create_fn_t)();
typedef void (*destroy_fn_t)(PluginPtr);

NativeLoaderResult load_native_plugin(const PluginManifest &m, const string &libpath) {
    NativeLoaderResult res;
#if defined(_WIN32)
    HMODULE h = LoadLibraryA(libpath.c_str());
    if (!h) {
        cerr << "[NativeLoader] LoadLibrary failed: " << libpath << "\n";
        return res;
    }
    auto create = (create_fn_t)GetProcAddress(h, "create_plugin");
    if (!create) {
        cerr << "[NativeLoader] symbol create_plugin missing\n";
        FreeLibrary(h);
        return res;
    }
    res.handle = (void*)h;
    res.instance = create();
#else
    void* h = dlopen(libpath.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (!h) {
        cerr << "[NativeLoader] dlopen failed: " << dlerror() << "\n";
        return res;
    }
    auto create = (create_fn_t)dlsym(h, "create_plugin");
    if (!create) {
        cerr << "[NativeLoader] symbol create_plugin missing: " << dlerror() << "\n";
        dlclose(h);
        return res;
    }
    res.handle = h;
    res.instance = create();
#endif
    return res;
}

bool unload_native_plugin(NativeLoaderResult &r) 
{
    if (!r.instance) return true;
    // try to call destroy if present
#if defined(_WIN32)
    HMODULE h = (HMODULE)r.handle;
    auto destroy = (destroy_fn_t)GetProcAddress(h, "destroy_plugin");
    if (destroy) destroy(r.instance);
    FreeLibrary(h);
#else
    void* h = r.handle;
    auto destroy = (destroy_fn_t)dlsym(h, "destroy_plugin");
    if (destroy) destroy(r.instance);
    dlclose(h);
#endif
    r.instance = nullptr;
    r.handle = nullptr;
    return true;
}

} // namespace ibcs::plugin::native
