#include "e_isoworld.hpp"
#include "lua.h"
#include "lualib.h"

#define META_ISOLEVEL 1

class Lua_IsoLevel {

	static int get(lua_State* L)
	{
		IsoLevel* level = *((IsoLevel**)lua_touserdatatagged(L, 1, META_ISOLEVEL));
		string field_query = luaL_checkstring(L, 2);


		if (field_query == "width")
			lua_pushinteger(L, level->width);
		else if (field_query == "height")
			lua_pushinteger(L, level->height);
		else if (field_query == "name")
			lua_pushstring(L, level->name.c_str());
		else if (field_query == "alias")
			lua_pushstring(L, level->alias.c_str());
		else if (field_query == "flags")
		{
			int index = luaL_checkinteger(L, 3);
			
			if (level->flags.size() < index)
				lua_pushboolean(L, level->flags[index]);
			else
				lua_pushnil(L);
		}
		else if (field_query == "levelflags")
		{
			int index = luaL_checkinteger(L, 3);

			if (level->levelflags.size() < index)
				lua_pushboolean(L, level->levelflags[index]);
			else
				lua_pushnil(L);
		}

		return 1;
	}

	static int set(lua_State* L)
	{
		IsoLevel* level = *((IsoLevel**)lua_touserdatatagged(L, 1, META_ISOLEVEL));
		string field_query = luaL_checkstring(L, 2);

		if (field_query == "flags")
		{
			int index = luaL_checkinteger(L, 3);

			if (level->flags.size() < index)
				level->flags[index] = luaL_checkinteger(L, 4);
			else
				std::cerr << "Invalid Index in Iso Level Userdata!" << std::endl;
		}
		else if (field_query == "levelflags")
		{
			int index = luaL_checkinteger(L, 3);

			if (level->levelflags.size() < index)
				level->levelflags[index] = luaL_checkinteger(L, 4);
			else
				std::cerr << "Invalid Index in Iso Level Userdata!" << std::endl;
		}

		return 1;
	}
};