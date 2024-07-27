#include <iostream>
#include <vector>
#include <filesystem>
#include <lua.h>

using namespace std;

std::string readFile(filesystem::path path);

void loadLuaFolder(lua_State* L, filesystem::path initpath, filesystem::path path);
void loadDataFolder(filesystem::path path);

void loadData(filesystem::path path);
