#include <iostream>
#include <fstream>
#include <filesystem>

#include "e_filesys.h"
#include "e_filepaths.h"

#include <lua.h>
#include <lualib.h>
#include <luacode.h>

std::vector<std::string*>sound_paths;
std::vector<std::string*>model_paths;
std::vector<std::string*>sprite_paths;
std::vector<std::string*>level_paths;

std::string readFile(std::filesystem::path path)
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

void loadLuaString(lua_State* L, const char* name, const char* stringscript)
{
	size_t outsize;
	char* script = luau_compile(stringscript, strlen(stringscript), nullptr, &outsize);

	lua_State* thread = lua_newthread(L);
	luaL_sandboxthread(thread);

	if (outsize > 0)
	{
		int err = luau_load(thread, name, script, outsize, 0);
		free(script);
		if (err == 1)
		{
			std::cout << lua_tostring(thread, -1) << std::endl;
			lua_resetthread(thread);
		}
		else
		{
			std::cout << "Script: " << name << " is Loaded" << std::endl;
			lua_resume(thread, NULL, 0);
		}
	}
}

static void loadLuaFile(lua_State* L, std::filesystem::path path)
{
	std::string strtext = readFile(path);
	const char* text = strtext.c_str();

	if (text[0] != '\0')
	{
		std::string filename = path.filename().string();
		loadLuaString(L, filename.c_str(), text);
	}
}


int searchscripts = 0;

void loadLuaFolder(lua_State* L, std::filesystem::path initpath, std::filesystem::path path)
{
	if (!std::filesystem::exists(initpath))
	{
		for (auto scriptfile = std::filesystem::recursive_directory_iterator(path); scriptfile != std::filesystem::recursive_directory_iterator(); scriptfile++)
		{
			std::string std_extension = scriptfile->path().filename().extension().string();

			if (scriptfile->is_regular_file() && (std_extension == ".lua" || std_extension == ".luau"))
				loadLuaFile(L, scriptfile->path());
			else
				continue;

			++searchscripts;
		}

		std::cout << "found lua scripts: " << searchscripts << std::endl;
	}
	else
	{
		loadLuaFile(L, initpath);
	}
}

void loadDataFolder(std::filesystem::path path)
{
	for (auto data = std::filesystem::recursive_directory_iterator(path); data != std::filesystem::recursive_directory_iterator(); data++)
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