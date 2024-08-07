#include "lualib.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <time.h>
#include <SDL.h>
#include "e_fps.h"

extern float FPS;
extern float deltaTime;

static int engine_print(lua_State* L)
{
    int n = lua_gettop(L); // number of arguments
    for (int i = 1; i <= n; i++)
    {
        size_t l;
        const char* s = luaL_tolstring(L, i, &l); // convert to string using __tostring et al
        if (i > 1)
            std::cout << "\t";
        std::cout << s;
        lua_pop(L, 1); // pop result
    }
    std::cout << "\n";
    return 0;
}

static int engine_deltatime(lua_State* L)
{
    lua_pushnumber(L, deltaTime);
    return 1;
}

static int engine_fps(lua_State* L)
{
    lua_pushnumber(L, FPS);
    return 1;
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
