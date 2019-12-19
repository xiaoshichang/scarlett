#include "Runtime/RHI/opengl/ShaderGL.h"
#include "Foundation/Assert.h"
#include "Runtime/Utils/Logging.h"
#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


scarlett::ShaderGL::ShaderGL(const std::string & vsPath, const std::string & psPath)
{
	InitializeFromFile(vsPath, psPath);
}

scarlett::ShaderGL::~ShaderGL()
{
	glDeleteProgram(mProgram);
}

bool scarlett::ShaderGL::InitializeFromFile(const std::string & vsPath, const std::string & psPath) noexcept
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vsPath);
		fShaderFile.open(psPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		SCARLETT_LOG(error) << "create shader fail";
		SCARLETT_ASSERT(false);
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		SCARLETT_LOG(error) << infoLog << vsPath;
		SCARLETT_ASSERT(false);
	};

	// similiar for Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		SCARLETT_LOG(error) << infoLog << psPath;
		SCARLETT_ASSERT(false);
	};

	// shader Program
	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);
	glLinkProgram(mProgram);
	// print linking errors if any
	glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(mProgram, 512, NULL, infoLog);
		SCARLETT_LOG(error) << infoLog;
		SCARLETT_ASSERT(false);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return 0;
}

void scarlett::ShaderGL::Use() noexcept
{
	glUseProgram(mProgram);
}


void scarlett::ShaderGL::SetConstantBuffer(const ConstantBuffer & cbuffer) noexcept
{
	unsigned int location;
	location = glGetUniformLocation(mProgram, "worldMatrix");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, true, cbuffer.world);
	}

	location = glGetUniformLocation(mProgram, "viewMatrix");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, true, cbuffer.view);
	}

	location = glGetUniformLocation(mProgram, "projectionMatrix");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, true, cbuffer.projection);
	}

	location = glGetUniformLocation(mProgram, "inputColor");
	if (location != -1) {
		glUniform4fv(location, 1, cbuffer.debugColor);
	}
}

void scarlett::ShaderGL::SetConstantBufferLight(const ConstantBufferLighting & cbuffer) noexcept
{
}

void scarlett::ShaderGL::SetConstantBufferAnimation(const ConstantBufferAnimation & cbuffer) noexcept
{
}
