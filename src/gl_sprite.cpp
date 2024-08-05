#include <iostream>
#include <string>

#include <gl/glew.h>
#include "gl_sprite.hpp"
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

void Sprite::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

void Sprite::flush()
{
	glDeleteTextures(1, &TextureID);
}
