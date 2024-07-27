// This will be used mostly for HUD drawing, and maybe some additions inlevel.
#include "lualib.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <time.h>
#include <SDL.h>

#include "lua_render.h"
using namespace std;

static int gl_createShader(lua_State* L)
{
    return 0;
}

static const luaL_Reg enginegl_funcs[] = {
    {"createShader", gl_createShader},
    {NULL, NULL},
};

int enginelua_gl(lua_State* L)
{
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, "gl", enginegl_funcs);
    return 1;
}
