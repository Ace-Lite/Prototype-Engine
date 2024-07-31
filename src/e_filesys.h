#include <iostream>
#include <vector>
#include <filesystem>
#include <lua.h>

using namespace std;

std::string readFile(filesystem::path path);

void loadLuaString(lua_State* L, const char* name, const char* stringscript);
void loadLuaFolder(lua_State* L, filesystem::path initpath, filesystem::path path);
void loadDataFolder(filesystem::path path);

void loadData(filesystem::path path);
