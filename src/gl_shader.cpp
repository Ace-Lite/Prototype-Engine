#include <iostream>
#include <string>
#include <filesystem>

// OpenGL
#include "gl/glew.h"
#include "gl_shader.hpp"

#include "e_filesys.h"

void Shader::compile(std::filesystem::path vertdata, std::filesystem::path fragdata)
{
	GLuint vertexshader, fragmentshader;
	GLint success = true;
	GLchar log[512];

	//
	// VERTEX SHADER
	//

	try
	{
		std::string source = readFile(vertdata);

		const char* c_source = source.c_str();

		vertexshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexshader, 1, &c_source, nullptr);
		glCompileShader(vertexshader);


		glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexshader, 512, nullptr, log);
			std::cerr << "Vertex shader couldn't be compiled | " << source << std::endl << log << std::endl;
			exit(1);
		}
	}
	catch (exception e)
	{
		std::cerr << "Vertex Shader couldn't be loaded" << std::endl;
		exit(1);
	}

	//
	// FRAGMENT SHADER
	//

	try
	{
		std::string source = readFile(fragdata);

		const char* c_source = source.c_str();

		fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentshader, 1, &c_source, nullptr);
		glCompileShader(fragmentshader);


		glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentshader, 512, nullptr, log);
			std::cerr << "Fragment shader couldn't be compiled | " << source << std::endl << log << std::endl;
			exit(1);
		}
	}
	catch (exception e)
	{
		std::cerr << "Fragment Shader couldn't be loaded" << std::endl;
		exit(1);
	}

	// Porgram

	id_ = glCreateProgram();
	glAttachShader(id_, vertexshader);
	glAttachShader(id_, fragmentshader);

	glLinkProgram(id_);

	glGetProgramiv(id_, GL_LINK_STATUS, & success);
	if (!success)
	{
		glGetProgramInfoLog(id_, 512, nullptr, log);
		std::cerr << "Program linking error" << std::endl << log << std::endl;
		exit(1);
	}

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
}

void Shader::activate()
{
	glUseProgram(id_);
}

void Shader::setBool(const GLchar* name, GLboolean value)
{
	glUniform1i(glGetUniformLocation(id_, name), value);
}

void Shader::setInt(const GLchar* name, GLint value)
{
	glUniform1i(glGetUniformLocation(id_, name), value);
}

void Shader::setFloat(const GLchar* name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(id_, name), value);
}

void Shader::setVec2f(const GLchar* name, GLfloat x, GLfloat y)
{
	glUniform2f(glGetUniformLocation(id_, name), x, y);
}

void Shader::setVec3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}

void Shader::setVec4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
	glUniform4f(glGetUniformLocation(id_, name), x, y, z, a);
}