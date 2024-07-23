#include "lualib.h"

#define LUA_ENGINEBASE "engine"
LUALIB_API int enginelua_base(lua_State* L);

void luaL_enginelibs(lua_State* L);