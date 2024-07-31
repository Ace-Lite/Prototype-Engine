#include "lua.h"
#include "lualib.h"
#include "lua_sdl.h"
#include <string>
#include <vector>
#include <iostream>
#include <SDL.h>
using namespace std;

static int lualib_getWindowSize(lua_State* L)
{
	Uint32 tics = SDL_GetTicks64();
	lua_pushinteger(L, tics);
	lua_pop(L, 1);
	return 0;
}

static const luaL_Reg enginesdl_funcs[] = {
	{"getWindowSize", lualib_getWindowSize},
	{NULL, NULL},
};

int enginelua_sdl(lua_State* L)
{
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	luaL_register(L, LUA_ENGINESDL, enginesdl_funcs);
	lua_pop(L, 1);
	return 1;
}
