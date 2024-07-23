#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
using namespace std;

class Image {

	public:
		SDL_Surface* surface;
		int imgFlags = IMG_INIT_PNG;
		bool success = false;

		int init(SDL_Window* window)
		{
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				cout << "SDL2 couldn't initialize | SDL2 Image error:" << IMG_GetError() << endl;
				return 0;
			}
			else
			{
				surface = SDL_GetWindowSurface(window);
				return 1;
			}
		}


		SDL_Surface* cacheImage(std::string path)
		{
			SDL_Surface* optimizedSurface = nullptr;

			SDL_Surface* loadedSurface = IMG_Load(path.c_str());
			if (loadedSurface == nullptr)
				cout << "Unable to load image " << path.c_str() << " SDL2 Image error: " << IMG_GetError() << endl;
			else
			{
				optimizedSurface = SDL_ConvertSurface(loadedSurface, surface->format, 0);

				if (optimizedSurface == nullptr)
					cout << "Unable to optimize Image " << path.c_str() << ". SDL2 error: " << SDL_GetError() << endl;


				SDL_FreeSurface(surface);
			}

			return optimizedSurface;
		}



		int render()
		{




		}
};