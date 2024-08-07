#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>

#define DEBUG_MODE 1
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
#include "gl_init.hpp"
#include "SDL_opengl.h"

// Engine Headers
#include "e_filesys.h"
#include "e_fps.h"

SDL_GLContext gContext;
extern lua_State* L;

bool gRenderQuad = true;

const int WIN_WIDTH = 400;
const int WIN_HEIGHT = 400;

float deltaTime = 0;
float FPS = 0;

int main(int arg)
{
	//
	//	File System Declarations
	//

	std::string filepath = SDL_GetBasePath();

	std::filesystem::path luapath = filepath + "scripts";
	std::filesystem::path datapath = filepath + "data";

	if (!filesystem::exists(luapath))
	{
#ifdef DEBUG_MODE
		std::cout << "[Debug ERROR] No path to lua was found." << std::endl;
#endif
		return 0;
	}

	if (!filesystem::exists(datapath))
	{
#ifdef DEBUG_MODE
		std::cout << "[Debug ERROR] No path to data was found." << std::endl;
#endif
		return 0;
	}

#ifdef DEBUG_MODE
	std::cout << "[Debug NOTE] Path to lua & data was found!" << std::endl;
#endif

	loadDataFolder(datapath);

	//
	//	SDL Setup
	//

	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr;
	//SDL_Renderer* renderer = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error during SDL_Init: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	else {
		window = SDL_CreateWindow("Prototype Engine", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		if (window == nullptr) {
			std::cout << "Error during SDL_CreateWindow: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return -1;
		}
	}

	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) < 0) {
		std::cout << "Error during SDL_Mix_Init: " << Mix_GetError() << std::endl;
		Mix_Quit();
	}


	//
	// GL Renderer Setup
	//

	//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	gContext = SDL_GL_CreateContext(window);

	if (gContext == nullptr)
	{
		std::cout << "Error during SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
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
			std::cout << "Error during initiation of GLEW: " << glewGetErrorString(glewError) << std::endl;
			SDL_Delay(115000);
			Mix_Quit();
			SDL_Quit();
			return -1;
		}

		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			std::cout << "Warning! Unable to set VSync. SDL2 Error: " << SDL_GetError() << std::endl;
			SDL_Delay(25);
		}

		if (SDL_GL_MakeCurrent(window, gContext) < 0)
		{
			std::cout << "Warning! Unable to set current of GL. SDL2 Error: " << SDL_GetError() << std::endl;
			SDL_Delay(25);

		}

		if (!initGL())
		{
			std::cout << "Error while initializing OpenGL." << std::endl;
			SDL_Delay(115000);
			Mix_Quit();
			SDL_Quit();
			return -1;
		}

	}

	startLua();
	std::filesystem::path initlua = filepath + "scripts\\init.luau";
	loadLuaFolder(L, initlua, luapath);

	GL_loadDefaultContent(datapath.string());
	GL_test();

	//
	//	Events
	//

	SDL_Event e;
	bool quit = false;
	while (quit == false) {

		Uint32 startOfTick = SDL_GetTicks();
		Uint64 startOfFrame = SDL_GetPerformanceCounter();

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
		
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				//cout << lua_gettop(L) << std::endl;
				events_keydown_press(L, SDL_GetKeyName(e.key.keysym.sym));
				break;
			case SDL_KEYUP:
				events_keyup_press(L, SDL_GetKeyName(e.key.keysym.sym));
				break;


			// unused
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				break;

			// reset
			default:
				//events_keydown_press(L, "NULL");
				//events_keyup_press(L, "NULL");
				//events_quit(L, quit);
				break;
			}
		}

		events_thinkframe(L, deltaTime);
		GL_rendererRender(window);

		Uint32 endOfTick = SDL_GetTicks();
		Uint64 endOfFrame = SDL_GetPerformanceCounter();
		FPS = 1.0f / ((endOfFrame - startOfFrame) / (float)SDL_GetPerformanceFrequency());
		deltaTime = (endOfTick - startOfTick) / 1000.0f;

		if (quit == true)
		{
			// Close Lua for apperant reasons
			shutdownLua(L);

			SDL_DestroyWindow(window);
			Mix_Quit();
			SDL_Quit();
			return 0;
		}
	}

	return 0;
}