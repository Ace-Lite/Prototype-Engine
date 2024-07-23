#include "lualib.h"
#include <iostream>
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

static const luaL_Reg enginebase_funcs[] = {
    {"print", engine_print},
    {NULL, NULL},
};

int enginelua_base(lua_State* L)
{
    luaL_register(L, "base", enginebase_funcs);
    return 1;
}
