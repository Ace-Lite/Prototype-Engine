#include <iostream>
#include <fstream>
#include <filesystem>

#include <SDL.h>

#include "gl/glew.h"
#include "gl_init.hpp"

#include "gl_sprite.hpp"
#include "gl_shader.hpp"

#include "e_filesys.h"
#include "e_filepaths.h"

#include "entities.hpp"

using namespace std;

GLuint gVBO = 0;
GLuint gEBO = 0;

Sprite *errorSprite;
Shader *defaultShader;


extern class EntityManager;

bool initGL()
{
	// Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool noerr = true;

	GLint success = true;
	GLchar log[512];

	
	//VBO data
	GLfloat vertexData[] =
	{
		-0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f,	0.0f, 1.0f,
	
		-0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,	1.0f, 1.0f
	};

	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };

	//Create Vertex Buffer
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	//Create Element Buffer
	glGenBuffers(1, &gEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	return noerr;
}

bool GL_loadDefaultContent(string path)
{
	try
	{
		string errorSpritePath = path + "\\sprites\\error.png";
		Sprite errorNewSprite(&errorSpritePath);
		errorSprite = &errorNewSprite;

		filesystem::path defaultVertexShaderPath = path + "\\shaders\\main.vert";
		filesystem::path defaultFragmeShaderPath = path + "\\shaders\\main.frag";

		defaultShader = new Shader();
		defaultShader->compile(defaultVertexShaderPath, defaultFragmeShaderPath);
		return true;
	}
	catch (exception e)
	{
		return false;
	}
}

void GL_test()
{
	Object* obj1 = EntityManager::createObject();
	obj1->pos = {32.0f, 32.0f};
	obj1->sprite = errorSprite;
	obj1->shader = defaultShader;

	Object* obj2 = EntityManager::createObject();
	obj2->pos = { 128.0f, 52.0f };
	obj2->sprite = errorSprite;
	obj2->shader = defaultShader;
}

bool GL_rendererRender(SDL_Window* window)
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	//GLsizei entityCount = EntityManager::returnCount();
	//GLsizei tilesCount = 0;

	//Set index data and render
	//glDrawArrays(GL_TRIANGLES, 0, (tilesCount + entityCount) * 6);
	EntityManager::draw();

	SDL_GL_SwapWindow(window);

	return true;
}