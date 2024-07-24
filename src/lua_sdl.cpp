#include "lua.h"
#include "lualib.h"
#include "lua_sdl.h"
#include <string>
#include <vector>
#include <iostream>
#include <SDL.h>
using namespace std;

static int lualib_getTicks32(lua_State* L)
{
	Uint32 tics = SDL_GetTicks();
	lua_pushinteger(L, tics);
	return 0;
}

static int lualib_getTicks64(lua_State* L)
{
	Uint32 tics = SDL_GetTicks64();
	lua_pushinteger(L, tics);
	return 0;
}

static int lualib_getWindowSize(lua_State* L)
{
	Uint32 tics = SDL_GetTicks64();
	lua_pushinteger(L, tics);
	return 0;
}

static const luaL_Reg enginesdl_funcs[] = {
	{"getTicks", lualib_getTicks32},
	{"getTicks64", lualib_getTicks64},

	{"getWindowSize", lualib_getTicks64},
	{NULL, NULL},
};

int enginelua_sdl(lua_State* L)
{
	luaL_register(L, LUA_ENGINESDL, enginesdl_funcs);
	return 1;
}
