#include <iostream>
#include <vector>
#include <filesystem>
#include <lua.h>

std::string readFile(std::filesystem::path path);

void loadLuaString(lua_State* L, const char* name, const char* stringscript);
void loadLuaFolder(lua_State* L, std::filesystem::path initpath, std::filesystem::path path);
void loadDataFolder(std::filesystem::path path);

void loadData(std::filesystem::path path);
