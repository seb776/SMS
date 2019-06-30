#include <Tools/GLHelper.h>
#include <dependencies/gl/glext.h>
#include <Tools/MeanLeanWindows.h>
#include <Tools/Memory.h>
#include "FragmentShader.h"

using namespace Discrepancy;

FragmentShader::FragmentShader() {

}

int mystrlen(const char *buf)
{
	int i = 0;
	for (i = 0; buf[i] != '\0'; ++i)
		;
	return i;
}

bool FragmentShader::Load(const char code[]) {
	auto lenshader = mystrlen(code);
	_shaderCode = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, lenshader + 1);
	Memory::MemCpy(_shaderCode, code, lenshader + 1);

	_shaderId = glCreateShader(GL_FRAGMENT_SHADER);
	const GLint len = (GLint)mystrlen(_shaderCode);

	glShaderSource(_shaderId, 1, (const GLchar *const*)&_shaderCode, &len);

	glCompileShader(_shaderId);
	GLint status = 0;
	glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(_shaderId, GL_INFO_LOG_LENGTH, &maxLength);
		char *buffer = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, maxLength);
		glGetShaderInfoLog(_shaderId, maxLength, &maxLength, &buffer[0]);
		OutputDebugString("compile shader info log");
		OutputDebugString((char*)buffer);
		glGetProgramInfoLog(_shaderId, maxLength, &maxLength, &buffer[0]);
		OutputDebugString("compile program info log");
		OutputDebugString((char*)buffer);
		return false;
	}

	_programId = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(_programId, _shaderId);
	// Link our program
	glLinkProgram(_programId);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programId, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

		glGetShaderiv(_shaderId, GL_INFO_LOG_LENGTH, &maxLength);
		char *buffer = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, maxLength);

		glGetProgramInfoLog(_shaderId, maxLength, &maxLength, &buffer[0]);

		OutputDebugString("link program info log");
		OutputDebugString((char*)buffer);
		return false;
	}
	return true;
}


void FragmentShader::Render(ShaderParameters parameters) 
{
	 // TODO handle parameters passing and texture exchange
	glUseProgram(_programId);
}


FragmentShader::~FragmentShader() {

}