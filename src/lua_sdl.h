#include <iostream>
#include <string>

#include "lualib.h"

#define LUA_ENGINESDL "sdl"
LUALIB_API int enginelua_sdl(lua_State* L);

void luaL_sdllibs(lua_State* L);