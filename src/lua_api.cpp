#include "lualib.h"
#include <stdlib.h>

#include "lua_api.h"
#include "lua_sdl.h"
#include "lua_events.h"

static const luaL_Reg enginelibs[] = {
    {LUA_ENGINEBASE, enginelua_base},
	{LUA_ENGINEEVENTS, enginelua_events},

	{LUA_ENGINESDL, enginelua_sdl},
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

void luaL_eventslibs(lua_State* L)
{
	const luaL_Reg* lib = enginelibs;
	for (; lib->func; lib++)
	{
		lua_pushcfunction(L, lib->func, NULL);
		lua_pushstring(L, lib->name);
		lua_call(L, 1, 0);
	}
}

void luaL_sdllibs(lua_State* L)
{
	const luaL_Reg* lib = enginelibs;
	for (; lib->func; lib++)
	{
		lua_pushcfunction(L, lib->func, NULL);
		lua_pushstring(L, lib->name);
		lua_call(L, 1, 0);
	}
}

