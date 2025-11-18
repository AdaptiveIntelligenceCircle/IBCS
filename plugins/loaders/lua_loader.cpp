#include "lua_loader.h"
using namespace std;

#include <iostream>
#include <memory>

#include <fstream> 
#include <sstream> 

#ifdef __has_include
#  if __has_include(<lua.hpp>)
#    include <lua.hpp>
#    define IBCS_HAVE_LUA 1
#  elif __has_include("lua.h")
#    include "lua.h"
#    include "lauxlib.h"
#    include "lualib.h"
#    define IBCS_HAVE_LUA 1
#  else
#    define IBCS_HAVE_LUA 0
#  endif
#else
#  define IBCS_HAVE_LUA 0
#endif

namespace ibcs::plugin::lua_loader 
{

using namespace ibcs::plugin;

#if IBCS_HAVE_LUA

// C++ shim implementing IPlugin by delegating to Lua table with functions:
// plugin.info(), plugin.on_load(), plugin.on_unload(), plugin.capabilities()
class LuaShim : public IPlugin 
{
public:
    LuaShim(lua_State* L, int table_ref) : L_(L), table_ref_(table_ref) {}
    ~LuaShim() override 
    {
        // unref table
        luaL_unref(L_, LUA_REGISTRYINDEX, table_ref_);
    }

    PluginInfo info() const override 
    {
        PluginInfo pi;
        lua_rawgeti(L_, LUA_REGISTRYINDEX, table_ref_); // push table
        lua_getfield(L_, -1, "info");
        if (lua_isfunction(L_, -1)) {
            lua_pushvalue(L_, -2); // push table as self
            if (lua_pcall(L_, 1, 1, 0) == LUA_OK) 
            {
                if (lua_istable(L_, -1)) 
                {
                    lua_getfield(L_, -1, "name"); if (lua_isstring(L_, -1)) pi.name = lua_tostring(L_, -1); lua_pop(L_,1);
                    lua_getfield(L_, -1, "version"); if (lua_isstring(L_, -1)) pi.version = lua_tostring(L_, -1); lua_pop(L_,1);
                    lua_getfield(L_, -1, "author"); if (lua_isstring(L_, -1)) pi.author = lua_tostring(L_, -1); lua_pop(L_,1);
                }
                lua_pop(L_,1); // ret
            } 
            else 
            {
                lua_pop(L_,1); // error
            }
        } 
        else 
        {
            lua_pop(L_,1); // not function
        }
        lua_pop(L_,1); // table
        return pi;
    }

    bool on_load() override 
    {
        bool ok = false;
        lua_rawgeti(L_, LUA_REGISTRYINDEX, table_ref_);
        lua_getfield(L_, -1, "on_load");
        if (lua_isfunction(L_, -1)) 
        {
            lua_pushvalue(L_, -2);
            if (lua_pcall(L_, 1, 1, 0) == LUA_OK) 
            {
                ok = lua_toboolean(L_, -1);
                lua_pop(L_,1);
            } 
            else 
            {
                lua_pop(L_,1);
            }
        } 
        else lua_pop(L_,1);
        lua_pop(L_,1);
        return ok;
    }

    bool on_unload() override 
    {
        bool ok = false;
        lua_rawgeti(L_, LUA_REGISTRYINDEX, table_ref_);
        lua_getfield(L_, -1, "on_unload");
        if (lua_isfunction(L_, -1)) 
        {
            lua_pushvalue(L_, -2);
            if (lua_pcall(L_, 1, 1, 0) == LUA_OK) 
            {
                ok = lua_toboolean(L_, -1);
                lua_pop(L_,1);
            } 
            else 
            {
                lua_pop(L_,1);
            }
        } 
        else lua_pop(L_,1);
        lua_pop(L_,1);
        return ok;
    }

    vector<string> capabilities() const override 
    {
        vector<string> caps;
        lua_rawgeti(L_, LUA_REGISTRYINDEX, table_ref_);
        lua_getfield(L_, -1, "capabilities");
        if (lua_istable(L_, -1)) 
        {
            lua_pushnil(L_);
            while (lua_next(L_, -2)) 
            {
                if (lua_isstring(L_, -1)) caps.push_back(lua_tostring(L_, -1));
                lua_pop(L_,1);
            }
        }
        lua_pop(L_,2);
        return caps;
    }

private:
    lua_State* L_;
    int table_ref_;
};

LuaPluginShim load_lua_plugin(const PluginManifest &m, const string &script_path) 
{
    LuaPluginShim res;
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // load script
    if (luaL_dofile(L, script_path.c_str()) != LUA_OK) 
    {
        cerr << "[LuaLoader] load failed: " << lua_tostring(L, -1) << "\n";
        lua_close(L);
        return res;
    }
    // Expect plugin table returned or global 'plugin' table
    if (lua_isnil(L, -1)) lua_pop(L,1);
    int table_index = -1;
    if (lua_istable(L, -1)) 
    {
        lua_pushvalue(L, -1);
        table_index = luaL_ref(L, LUA_REGISTRYINDEX);
        lua_pop(L,1);
    } 
    else 
    {
        // try global plugin
        lua_getglobal(L, "plugin");
        if (!lua_istable(L, -1)) 
        {
            cerr << "[LuaLoader] script did not return plugin table nor define global 'plugin'\n";
            lua_close(L);
            return res;
        }
        lua_pushvalue(L, -1);
        table_index = luaL_ref(L, LUA_REGISTRYINDEX);
        lua_pop(L,1);
    }

    // create shim
    LuaShim* shim = new LuaShim(L, table_index);
    res.instance = shim;
    res.lua_state = (void*)L;
    return res;
}

bool unload_lua_plugin(LuaPluginShim &r) 
{
    if (!r.instance) return true;
    // call on_unload
    r.instance->on_unload();
    // delete shim -> will unref table
    delete r.instance;
    // close lua state
    if (r.lua_state) 
    {
        lua_close((lua_State*)r.lua_state);
        r.lua_state = nullptr;
    }
    r.instance = nullptr;
    return true;
}

#else // no Lua available: provide safe stub

LuaPluginShim load_lua_plugin(const PluginManifest &m, const string &script_path) {
    cerr << "[LuaLoader] Lua not available at compile-time. Returning stub.\n";
    return LuaPluginShim{};
}
bool unload_lua_plugin(LuaPluginShim &r) { (void)r; return true; }

#endif

} // namespace ibcs::plugin::lua_loader