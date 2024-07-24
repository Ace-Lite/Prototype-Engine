#include "lua.h"
#include "lualib.h"
#include "lua_events.h"
#include <string>
#include <vector>
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
			lua_settop(L, 0);

			lua_getref(L, event_func);
			int store = lua_ref(L, -1);
			
			lua_pushstring(L, key_down.c_str());
			if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
				cerr << "Error calling function " << event_func << ": " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			else
			{
				cout << "Activate " << event_func << endl;
			}
			lua_unref(L, event_func);
			event_func = store;
		}
};

vector<keydown*> keydown_events;

class keyup : public luau_event
{
	public:

		void callFunc(lua_State* L, string key_up)
		{
			lua_settop(L, 0);

			lua_getref(L, event_func);
			int store = lua_ref(L, -1);

			lua_pushstring(L, key_up.c_str());
			if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
				cerr << "Error calling function " << event_func << ": " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			else
			{
				cout << "Activate " << event_func << endl;
			}
			lua_unref(L, event_func);
			event_func = store;
		}
};

vector<keyup*> keyup_events;

class thinkframe : public luau_event
{
public:

	void callFunc(lua_State* L)
	{
		lua_settop(L, 0);

		lua_getref(L, event_func);
		int store = lua_ref(L, -1);

		if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
			cerr << "Error calling function " << event_func << ": " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		else
			event_func = store;

		lua_unref(L, event_func);
		event_func = store;
	}
};

vector<thinkframe*> thinkframe_events;

// TODO: make name upper/lower case, no need to be case sensitive here.
static int createEvent(lua_State* L)
{
	const char* constchname = luaL_checkstring(L, 1);
	string name = constchname;

	luaL_checktype(L, 2, LUA_TFUNCTION);
	int ref_int = lua_ref(L, 2);

	lua_rawseti(L, LUA_REGISTRYINDEX, ref_int);
	
	if (name == "keyDown" || name == "keydown")
	{
		keydown newevent;
		newevent.event_func = ref_int;
		keydown_events.push_back(&newevent);
	}
	else if (name == "keyUp" || name == "keyup")
	{
		keyup newevent;
		newevent.event_func = ref_int;
		keyup_events.push_back(&newevent);
	}
	else if (name == "thinkFrame")
	{
		thinkframe newevent;
		newevent.event_func = ref_int;
		thinkframe_events.push_back(&newevent);
	}
	else if (name == "scriptLoad")
	{
		lua_pcall(L, 1, 0, 0);
		lua_unref(L, ref_int);
	}

	lua_pop(L, 1);
	return 0;
}

void events_keydown_press(lua_State* L, std::string key_event)
{
	if (keydown_events.empty())
		cout << "No Keydown Events" << endl;
	else
		for (auto it = keydown_events.begin(); it != keydown_events.end(); ++it)
		{
			(*it)->callFunc(L, key_event);
		}
}

void events_keyup_press(lua_State* L, std::string key_event)
{
	if (keyup_events.empty())
		cout << "No Keyup Events" << endl;
	else
		for (auto it = keyup_events.begin(); it != keyup_events.end(); ++it)
		{
			(*it)->callFunc(L, key_event);
		}
}

void events_thinkframe(lua_State* L)
{
	if (!thinkframe_events.empty())
		for (auto it = thinkframe_events.begin(); it != thinkframe_events.end(); ++it)
		{
			(*it)->callFunc(L);
		}
}


static const luaL_Reg engineevents_funcs[] = {
	{"add", createEvent},
	{NULL, NULL},
};

int enginelua_events(lua_State* L)
{
	luaL_register(L, LUA_ENGINEEVENTS, engineevents_funcs);
	return 1;
}
