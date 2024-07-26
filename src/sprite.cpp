#include <iostream>
#include <string>

#include <gl/glew.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class Sprite {
	GLuint TextureID;
	SDL_Surface* Surface;
public:

	Sprite(string* path, string name)
	{
		Surface = IMG_Load(path->c_str());

		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		int Mode = GL_RGB;

		if (Surface->format->BytesPerPixel == 4)
		{
			Mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Surface = nullptr;
	}

	void render(int x, int y, int width, int height)
	{
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(x,			y,			0);
			glTexCoord2f(1, 0); glVertex3f(x + width,	y,			0);
			glTexCoord2f(1, 0); glVertex3f(x + width,	y + height, 0);
			glTexCoord2f(0, 1); glVertex3f(x,			y + height, 0);
		glEnd();
	}

	void flush()
	{
		glDeleteTextures(1, &TextureID);
	}
};