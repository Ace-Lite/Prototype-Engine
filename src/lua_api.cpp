#include "lua.h"
#include "lualib.h"
#include <stdlib.h>

#include "e_filesys.h"

#include "lua_api.h"
#include "lua_sdl.h"
#include "lua_render.h"
#include "lua_events.h"

lua_State* L;

static const luaL_Reg enginelibs[] = {
    {LUA_ENGINEBASE, enginelua_base},
    {LUA_ENGINEEVENTS, enginelua_events},

	{LUA_ENGINESDL, enginelua_sdl},
    {LUA_ENGINEGL, enginelua_gl},
    {NULL, NULL},
};

void luaL_enginelibs(lua_State* L)
{
    const luaL_Reg* lib = enginelibs;
    
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func, NULL);
        lua_pushstring(L, lib->name);
        lua_call(L, 1, 0);
    }
}

void startLua()
{
    //
    //	Luau API
    //

    lua_State* opL = luaL_newstate();

    luaL_openlibs(opL);

    lua_settop(opL, 0);

    luaL_enginelibs(opL);

    luaL_sandbox(opL);

    L = opL;
}


