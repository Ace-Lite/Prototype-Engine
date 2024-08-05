#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include "gl_sprite.hpp"

bool initGL();

bool GL_rendererRender(SDL_Window* window);
bool GL_loadDefaultContent(std::string path);

void GL_test();

extern Sprite *errorSprite;