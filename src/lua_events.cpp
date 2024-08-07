#include "lua.h"
#include "lualib.h"
#include "lua_events.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class luau_event
{
	public:
		int event_func;
};

class keydown : public luau_event
{
	public:

		void callFunc(lua_State* L, string key_down)
		{
			lua_getref(L, event_func);

			lua_pushstring(L, key_down.c_str());
			if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
				std::cerr << "Error while calling function " << event_func << ": " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			//else
			//{
			//	cout << "Activate " << event_func << std::endl;
			//}
		}
};

vector<keydown*> keydown_events;

class keyup : public luau_event
{
	public:

		void callFunc(lua_State* L, string key_up)
		{
			lua_getref(L, event_func);

			lua_pushstring(L, key_up.c_str());
			if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
				std::cerr << "Error while calling function " << event_func << ": " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			//else
			//{
			//	cout << "Activate " << event_func << std::endl;
			//}
		}
};

vector<keyup*> keyup_events;

class thinkframe : public luau_event
{
public:

	void callFunc(lua_State* L, float deltaTime)
	{
		lua_getref(L, event_func);

		lua_pushnumber(L, deltaTime);
		if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
			std::cerr << "Error while calling function " << event_func << ": " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
};

vector<thinkframe*> thinkframe_events;

static int createEvent(lua_State* L)
{
	const char* constchname = luaL_checkstring(L, 1);
	string eventname = constchname;

	// lowercasing name string
	for (auto& c : eventname)
	{
		c = tolower(c);
	}

	luaL_checktype(L, 2, LUA_TFUNCTION);
	int id = lua_ref(L, 2);

	if (eventname == "keydown")
	{
		keydown *newevent = new keydown;
		newevent->event_func = id;
		keydown_events.push_back(newevent);
	}
	else if (eventname == "keyup")
	{
		keyup *newevent = new keyup;
		newevent->event_func = id;
		keyup_events.push_back(newevent);
	}
	else if (eventname == "thinkframe")
	{
		thinkframe *newevent = new thinkframe;
		newevent->event_func = id;
		thinkframe_events.push_back(newevent);
	}

	return 0;
}

void events_keydown_press(lua_State* L, std::string key_event)
{
	if (lua_gettop(L) > 0)
		lua_pop(L, -1);

	if (keydown_events.empty())
		cout << "No Keydown Events" << std::endl;
	else
	{
		for (auto it = keydown_events.begin(); it != keydown_events.end(); ++it)
		{
			(*it)->callFunc(L, key_event);
		}
	}
}

void events_keyup_press(lua_State* L, std::string key_event)
{
	if (lua_gettop(L) > 0)
		lua_pop(L, -1);

	if (keyup_events.empty())
		cout << "No Keyup Events" << std::endl;
	else
	{
		for (auto it = keyup_events.begin(); it != keyup_events.end(); ++it)
		{
			(*it)->callFunc(L, key_event);
		}
	}
}

void events_thinkframe(lua_State* L, float deltaTime)
{
	if (lua_gettop(L) > 0)
		lua_pop(L, -1);

	if (!thinkframe_events.empty())
	{
		for (auto it = thinkframe_events.begin(); it != thinkframe_events.end(); ++it)
		{
			(*it)->callFunc(L, deltaTime);
		}
	}
}

// This could just be deconstructor but uhh... Current lua design simply is 
void events_clean(lua_State* L)
{
	for (auto it = keydown_events.begin(); it != keydown_events.end(); ++it)
	{
		lua_unref(L, (*it)->event_func);
	}

	for (auto it = keyup_events.begin(); it != keyup_events.end(); ++it)
	{
		lua_unref(L, (*it)->event_func);
	}

	for (auto it = thinkframe_events.begin(); it != thinkframe_events.end(); ++it)
	{
		lua_unref(L, (*it)->event_func);
	}
}


static const luaL_Reg engineevents_funcs[] = {
	{"add", createEvent},
	{NULL, NULL},
};

int enginelua_events(lua_State* L)
{
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	luaL_register(L, LUA_ENGINEEVENTS, engineevents_funcs);
	lua_pop(L, 1);
	return 1;
}
