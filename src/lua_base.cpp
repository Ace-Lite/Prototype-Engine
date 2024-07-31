#include "lualib.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <time.h>
#include <SDL.h>
using namespace std;

static int engine_print(lua_State* L)
{
    int n = lua_gettop(L); // number of arguments
    for (int i = 1; i <= n; i++)
    {
        size_t l;
        const char* s = luaL_tolstring(L, i, &l); // convert to string using __tostring et al
        if (i > 1)
            cout << "\t";
        cout << s;
        lua_pop(L, 1); // pop result
    }
    cout << "\n";
    return 0;
}

Uint64 oldTime = 0;

static int deltaTime()
{
    Uint64 ticks = SDL_GetTicks64();

    Uint64 deltaTime = ticks - oldTime;
    oldTime = ticks;

    return deltaTime;
}

static int engine_deltatime(lua_State* L)
{
    lua_settop(L, -1);
    lua_pushnumber(L, deltaTime());
    lua_pop(L, 1); // pop result
    return 0;
}

static int engine_fps(lua_State* L)
{
    double deltatime = (1.0f / (double)deltaTime()) * 1000.0f;

    lua_pushnumber(L, deltatime);
    lua_pop(L, 1); // pop result
    return 0;
}

static const luaL_Reg enginebase_funcs[] = {
    {"print", engine_print},


    {"fps", engine_fps},
    {"deltatime", engine_deltatime},
    {NULL, NULL},
};

int enginelua_base(lua_State* L)
{
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, "base", enginebase_funcs);
    lua_pop(L, 1);
    return 1;
}
