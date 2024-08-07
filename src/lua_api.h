#include "lualib.h"
#include <string>

#define LUA_ENGINEBASE "engine"
LUALIB_API int enginelua_base(lua_State* L);

extern lua_State* L;

void startLua();
void shutdownLua(lua_State* L);