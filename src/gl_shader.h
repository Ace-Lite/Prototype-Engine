#pragma once
#include <iostream>
#include <string>

// OpenGL
#include "gl/glew.h"

using namespace std;

class Shader {
	
	// Program ID
	GLuint id_;
	void compile(const GLchar* vertdata, const GLchar* fragdata);
public:

	// ... this is werid, why calling a non-function variable is even possible.
	Shader()
		: id_(0)
	{}
	~Shader()
	{
		if (id_)
			glDeleteProgram(id_);
		id_ = 0;
	}

	void activate();

	// Considering as of right now we don't have vector and matrix datatypes as of my knowledge.
	// And it is likely not necessary as we will be dealing most of the time with intergers anyway.
	void setBool(const GLchar* name, GLboolean value);
	
	void setInt(const GLchar* name, GLint value);
	void setFloat(const GLchar* name, GLfloat value);

	void setVec2f(const GLchar* name, GLfloat x, GLfloat y);
	void setVec3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
	void setVec4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat a);
};