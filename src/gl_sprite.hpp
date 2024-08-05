#pragma once
#include <iostream>
#include <string>

// OpenGL
#include "gl/glew.h"
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class Sprite {
	GLuint TextureID;
public:
	SDL_Surface* Surface;
	int width = 0;
	int height = 0;

	Sprite(string* path)
	{
		Surface = IMG_Load(path->c_str());
		if (!Surface)
			return;

		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		int Mode = GL_RGB;

		if (Surface->format->BytesPerPixel == 4)
		{
			Mode = GL_RGBA;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);
		glBindTexture(GL_TEXTURE_2D, 0);

		width = Surface->w;
		height = Surface->h;
	};

	void bind();

	void flush();
};