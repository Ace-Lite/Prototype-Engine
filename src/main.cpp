#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>

#define DEBUG_MODE 0
#define SDL_MAIN_HANDLED 1
#define GLEW_STATIC

#include <SDL.h>
//#include <SDL_mixer.h>

#include <lua.h>
#include <lualib.h>
#include <luacode.h>

// Engine Lua Libs
#include "lua_api.h"
#include "lua_sdl.h"
#include "lua_events.h"

// OpenGL
#include "gl/glew.h"
#include "SDL_opengl.h"

#include "r_glmain.h"

SDL_GLContext gContext;
GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

bool gRenderQuad = true;

using namespace std;

const int WIN_WIDTH = 400;
const int WIN_HEIGHT = 400;

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

bool initGL()
{
	bool noerr = true;

	gProgramID = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertexShaderSource[] = {
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};

	glShaderSource(vertexShader, 1, vertexShaderSource, nullptr);

	glCompileShader(vertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);

	if (vShaderCompiled != GL_TRUE)
	{
		cout << "Unable to compile vertex shader: " << vertexShader << endl;
		noerr = false;
	}
	else
	{
		glAttachShader(gProgramID, vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fragmentShaderSource[] =
		{
			"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
		};

		glShaderSource(fragmentShader, 1, fragmentShaderSource, nullptr);

		glCompileShader(fragmentShader);

		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			cout << "Unable to compile fragment shader: " << fragmentShader << endl;
			noerr = false;
		}
		else
		{
			glAttachShader(gProgramID, fragmentShader);

			glLinkProgram(gProgramID);

			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				cout << "Error linking program: " << gProgramID << endl;
				noerr = false;
			}
			else
			{

				gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
				if (gVertexPos2DLocation == -1)
				{
					cout << "LVertexPos2D is not a valid glsl program variable: " << fragmentShader << endl;
					noerr = false;
				}
				else
				{
					glClearColor(0.f, 0.f, 0.f, 1.f);

					//VBO data
					GLfloat vertexData[] =
					{
						-0.5f, -0.5f,
						 0.5f, -0.5f,
						 0.5f,  0.5f,
						-0.5f,  0.5f
					};

					//IBO data
					GLuint indexData[] = { 0, 1, 2, 3 };

					//Create VBO
					glGenBuffers(1, &gVBO);
					glBindBuffer(GL_ARRAY_BUFFER, gVBO);
					glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

					//Create IBO
					glGenBuffers(1, &gIBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
				}
			}
		}
	}

	return noerr;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (gRenderQuad)
	{

		glUseProgram(gProgramID);
		glEnableVertexAttribArray(gVertexPos2DLocation);

		glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

		//Set index data and render
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);

		//Disable vertex position
		glDisableVertexAttribArray(gVertexPos2DLocation);

		//Unbind program
		glUseProgram(NULL);
	}
}

int main(int arg)
{
	//
	//	File System Declarations
	//

	char const* filepath = SDL_GetBasePath();

	std::filesystem::path luapath;
	std::filesystem::path datapath;

	for (auto i = filesystem::directory_iterator(filepath); i != filesystem::directory_iterator(); i++)
	{
		if (i->is_directory())
		{
			std::string file_str = i->path().stem().string();
			std::string path_str = i->path().string();

			if (file_str == "scripts")
			{
				luapath = path_str;
			}
			else if (file_str == "data")
			{
				datapath = path_str;
			}
		}
		else
			continue;
	}

	if (!luapath.c_str())
	{
#ifdef DEBUG_MODE
		cout << "[Debug ERROR] No path to lua was found." << endl;
#endif
		return 0;
	}

	if (!datapath.c_str())
	{
#ifdef DEBUG_MODE
		cout << "[Debug ERROR] No path to data was found." << endl;
#endif
		return 0;
	}

#ifdef DEBUG_MODE
	cout << "[Debug NOTE] Path to lua & data was found!" << endl;
#endif

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

	//if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) < 0) {
	//	cout << "Error during SDL_Mix_Init: " << Mix_GetError() << endl;
	//	Mix_Quit();
	//}


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
		//Mix_Quit();
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
			//Mix_Quit();
			SDL_Quit();
			return -1;
		}

		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			cout << "Warning! Unable to set VSync. SDL2 Error: " << SDL_GetError() << endl;
			SDL_Delay(25);
		}

		if (!initGL())
		{
			cout << "Error while initializing OpenGL." << endl;
			SDL_Delay(115000);
			//Mix_Quit();
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
	luaL_eventslibs(L);
	luaL_sdllibs(L);

	for (auto scriptfile = filesystem::recursive_directory_iterator(luapath); scriptfile != filesystem::recursive_directory_iterator(); scriptfile++)
	{
		std::string std_extension = scriptfile->path().filename().extension().string();

		if (scriptfile->is_regular_file() && (std_extension == ".lua" || std_extension == ".luau"))
		{
			std::string text = readFile(scriptfile->path());
			const char* chartext = text.c_str();

			if (chartext[0] != '\0')
			{

				size_t outsize;
				std::string filename = scriptfile->path().filename().string();
				char* script = luau_compile(chartext, strlen(chartext), nullptr, &outsize);

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
						cout << "Script: " << filename.c_str() << " Loaded" << endl;
						lua_call(L, 0, 0);
					}
				}


			}
		}
		else
			continue;
	}


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
		
		render();
		SDL_GL_SwapWindow(window);

		if (quit == true)
		{
			// Close Lua for apperant reasons
			lua_close(L);

			SDL_DestroyWindow(window);
			//Mix_Quit();
			SDL_Quit();
			return 0;
		}
	}





	return 0;
}