#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>

#define DEBUG_MODE 0
#define SDL_MAIN_HANDLED 1
#define GLEW_STATIC

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <lua.h>
#include <lualib.h>
#include <luacode.h>

// Engine Lua Libs
#include "lua_api.h"
#include "lua_sdl.h"
#include "lua_events.h"

// OpenGL
#include "gl/glew.h"
#include "gl_init.h"
#include "SDL_opengl.h"

#include "gl_init.h"

// File loader
#include "e_filesys.h"

SDL_GLContext gContext;

bool gRenderQuad = true;

using namespace std;

const int WIN_WIDTH = 400;
const int WIN_HEIGHT = 400;

int main(int arg)
{
	//
	//	File System Declarations
	//

	string filepath = SDL_GetBasePath();

	std::filesystem::path luapath = filepath + "/scripts";
	std::filesystem::path datapath = filepath + "/data";

	if (!filesystem::exists(luapath))
	{
#ifdef DEBUG_MODE
		cout << "[Debug ERROR] No path to lua was found." << endl;
#endif
		return 0;
	}

	if (!filesystem::exists(datapath))
	{
#ifdef DEBUG_MODE
		cout << "[Debug ERROR] No path to data was found." << endl;
#endif
		return 0;
	}

#ifdef DEBUG_MODE
	cout << "[Debug NOTE] Path to lua & data was found!" << endl;
#endif

	loadDataFolder(datapath);

	//
	//	SDL Setup
	//

	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Renderer* renderer = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Error during SDL_Init: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}
	else {
		window = SDL_CreateWindow("Prototype Engine", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		if (window == nullptr) {
			cout << "Error during SDL_CreateWindow: " << SDL_GetError() << endl;
			SDL_Quit();
			return -1;
		}
	}

	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) < 0) {
		cout << "Error during SDL_Mix_Init: " << Mix_GetError() << endl;
		Mix_Quit();
	}


	//
	// GL Renderer Setup
	//

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	gContext = SDL_GL_CreateContext(window);

	if (gContext == NULL)
	{
		cout << "Error during SDL_GL_CreateContext: " << SDL_GetError() << endl;
		SDL_Delay(250);
		Mix_Quit();
		SDL_Quit();
		return -1;
	}
	else
	{
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();

		if (glewError != GLEW_OK)
		{
			cout << "Error during initiation of GLEW: " << glewGetErrorString(glewError) << endl;
			SDL_Delay(115000);
			Mix_Quit();
			SDL_Quit();
			return -1;
		}

		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			cout << "Warning! Unable to set VSync. SDL2 Error: " << SDL_GetError() << endl;
			SDL_Delay(25);
		}

		if (SDL_GL_MakeCurrent(window, gContext) < 0)
		{
			cout << "Warning! Unable to set current of GL. SDL2 Error: " << SDL_GetError() << endl;
			SDL_Delay(25);

		}

		if (!initGL())
		{
			cout << "Error while initializing OpenGL." << endl;
			SDL_Delay(115000);
			Mix_Quit();
			SDL_Quit();
			return -1;
		}

	}

	//
	//	Luau API
	//

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_enginelibs(L);

	std::filesystem::path initlua = luapath.string() + "/init.luau";
	loadLuaFolder(L, initlua, luapath);

	//
	//	Sprite Allocation
	//


	//
	//	Events
	//

	SDL_Event e;
	bool quit = false;
	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
		
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				events_keydown_press(L, SDL_GetKeyName(e.key.keysym.sym));
				break;
			case SDL_KEYUP:
				events_keyup_press(L, SDL_GetKeyName(e.key.keysym.sym));
				break;


			// unused
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			default:
				break;
			}
		}
		
		renderGL(window);
		SDL_GL_SwapWindow(window);

		// Lua
		events_thinkframe(L);

		if (quit == true)
		{
			// Close Lua for apperant reasons
			lua_close(L);

			SDL_DestroyWindow(window);
			Mix_Quit();
			SDL_Quit();
			return 0;
		}
	}





	return 0;
}