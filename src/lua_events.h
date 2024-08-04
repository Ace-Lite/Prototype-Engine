#include <iostream>
#include <string>

#include "lualib.h"

#define LUA_ENGINEEVENTS "events"
LUALIB_API int enginelua_events(lua_State* L);

void events_keydown_press(lua_State* L, std::string key_event);
void events_keyup_press(lua_State* L, std::string key_event);
void events_thinkframe(lua_State* L, float deltaTime);
void events_clean(lua_State* L);