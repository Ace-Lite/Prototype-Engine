// This will be used mostly for HUD drawing, and maybe some additions inlevel.
#include "lualib.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <time.h>
#include <SDL.h>

#include "lua_render.h"
#include "gl_shader.hpp"
#include "gl_sprite.hpp"
using namespace std;

static int gl_cacheShader(lua_State* L)
{
    string filepath = SDL_GetBasePath();

    filesystem::path vectShaderPath = filepath + "\\data\\" + luaL_checkstring(L, 1);
    filesystem::path fragShaderPath = filepath + "\\data\\" + luaL_checkstring(L, 2);

    Shader* shader = new Shader();
    shader->compile(vectShaderPath, fragShaderPath);
    return 0;
}

vector<Sprite*> customSpriteList;

static int gl_cacheSprite(lua_State* L)
{
    string filepath = SDL_GetBasePath();
    string spritePath = filepath + "\\data\\" + luaL_checkstring(L, 1);

    Sprite spriteEntry(&spritePath);
    customSpriteList.push_back(&spriteEntry);

    lua_pushinteger(L, customSpriteList.size());
    return 1;
}

static const luaL_Reg enginegl_funcs[] = {
    {"cacheShader", gl_cacheShader},
    {"cacheSprite", gl_cacheSprite},

    {NULL, NULL},
};

int enginelua_gl(lua_State* L)
{
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, "gl", enginegl_funcs);
    lua_pop(L, 1);
    return 1;
}
