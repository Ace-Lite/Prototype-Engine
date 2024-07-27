#include <iostream>
#include <fstream>
#include <filesystem>

#include <SDL.h>

#include "gl/glew.h"
#include "gl_init.h"

#include "e_filesys.h"
#include "e_filepaths.h"

using namespace std;

GLuint gVBO = 0;
GLuint gIBO = 0;

bool initGL()
{
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

	//Create VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	//Create IBO
	glGenBuffers(1, &gIBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	return noerr;
}

bool renderGL(SDL_Window* window)
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Set index data and render
	glDrawArrays(GL_QUADS, 0, 4);

	return true;
}