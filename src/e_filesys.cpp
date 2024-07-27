#include <iostream>
#include <fstream>
#include <filesystem>

#include "e_filesys.h"
#include "e_filepaths.h"

#include <lua.h>
#include <lualib.h>
#include <luacode.h>

using namespace std;

vector<string*>sound_paths;
vector<string*>model_paths;
vector<string*>sprite_paths;
vector<string*>level_paths;

std::string readFile(filesystem::path path)
{
	// Open the stream to 'lock' the file.
	std::ifstream f(path, std::ios::in | std::ios::binary);

	// Obtain the size of the file.
	const auto sz = filesystem::file_size(path);

	// Create a buffer.
	std::string result(sz, '\0');

	// Read the whole file into the buffer.
	f.read(result.data(), sz);

	return result;
}

static void loadLua(lua_State* L, filesystem::path path)
{
	std::string strtext = readFile(path);
	const char* text = strtext.c_str();

	if (text[0] != '\0')
	{
		size_t outsize;
		std::string filename = path.filename().string();
		char* script = luau_compile(text, strlen(text), nullptr, &outsize);

		if (outsize > 0)
		{
			int err = luau_load(L, filename.c_str(), script, outsize, 0);
			if (err == 1)
			{
				cout << lua_tostring(L, -1) << endl;
				free(script);
			}
			else
			{
				cout << "Script: " << filename.c_str() << " is Loading" << endl;
				if (lua_pcall(L, 0, LUA_MULTRET, lua_gettop(L) - 1))
				{
					cout << "Luau Warning:" << lua_tostring(L, -1) << endl;
					lua_pop(L, 1);
				}
			}
		}
	}
}


void loadLuaFolder(lua_State* L, filesystem::path initpath, filesystem::path path)
{
	if (!filesystem::exists(initpath))
	{
		for (auto scriptfile = filesystem::recursive_directory_iterator(path); scriptfile != filesystem::recursive_directory_iterator(); scriptfile++)
		{
			std::string std_extension = scriptfile->path().filename().extension().string();

			if (scriptfile->is_regular_file() && (std_extension == ".lua" || std_extension == ".luau"))
				loadLua(L, scriptfile->path());
			else
				continue;
		}
	}
	else
	{
		loadLua(L, initpath);
	}
}

void loadDataFolder(filesystem::path path)
{
	for (auto data = filesystem::recursive_directory_iterator(path); data != filesystem::recursive_directory_iterator(); data++)
	{
		std::string std_extension = data->path().filename().extension().string();

		if (data->is_regular_file())
		{
			if (std_extension == ".png")
				sprite_paths.push_back(&data->path().string());
				
			//if (std_extension == ".vert")
			//	vert_paths.push_back(data->path().string());
			//else if (std_extension == ".frag")
			//	frag_paths.push_back(data->path().string());
		}
	}
}