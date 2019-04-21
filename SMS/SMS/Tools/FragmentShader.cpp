//#include <GL\glew.h>
#include "MeanLeanWindows.h"
#include "GLHelper.h"
#include ".\..\dependencies\gl\glext.h"
#include "FragmentShader.h"
#include "Memory.h"

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
	auto len = mystrlen(code);
	_shaderCode = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len + 1);
	Memory::MemCpy(_shaderCode, code, len + 1);
	return true;
}


unsigned int FragmentShader::Run() {
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLint len = (GLint)mystrlen(_shaderCode);

	glShaderSource(shader, 1, (const GLchar *const*)&_shaderCode, &len);

	glCompileShader(shader);
	GLint status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		char *buffer = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &buffer[0]);
		OutputDebugString("compile shader info log");
		OutputDebugString((char*)buffer);
		glGetProgramInfoLog(shader, maxLength, &maxLength, &buffer[0]);
		OutputDebugString("compile program info log");
		OutputDebugString((char*)buffer);
	}

	GLuint program = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(program, shader);
	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		//std::vector<GLchar> infoLog(maxLength);

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		char *buffer = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, maxLength);

		glGetProgramInfoLog(program, maxLength, &maxLength, &buffer[0]);

		OutputDebugString("link program info log");
		OutputDebugString((char*)buffer);
	}
	glUseProgram(program);
	return program;
	//const unsigned int t = glCreateShaderProgram(GL_FRAGMENT_SHADER, _shaderCode);
	//
	//GLint status = 0;
	//glGetShaderiv(t, GL_COMPILE_STATUS, &status);
	//if (status == GL_FALSE)
	//{
	//	GLint maxLength = 0;
	//	glGetShaderiv(t, GL_INFO_LOG_LENGTH, &maxLength);
	//	char *buffer = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, maxLength);
	//	glGetShaderInfoLog(t, maxLength, &maxLength, &buffer[0]);
	//	glGetProgramInfoLog(t, maxLength, &maxLength, &buffer[0]);

	//	GLenum errCode;
	//	const GLubyte *errString;
	//	if ((errCode = glGetError()) !=
	//		GL_NO_ERROR)
	//	{
	//		errString = glGetString(errCode);
	//		OutputDebugString((char*)errString);
	//	}
	//	//return 0;
	//}

	//status = 0;
	//glGetShaderiv(t, GL_LINK_STATUS, &status);
	//if (status == GL_FALSE)
	//{
	//	GLenum errCode;
	//	const GLubyte *errString;
	//	if ((errCode = glGetError()) !=
	//		GL_NO_ERROR)
	//	{
	//		errString = glGetString(errCode);
	//		OutputDebugString((char*)errString);
	//	}
	//	//return 0;
	//}
	//glUseProgram(t);
	//return t;
}


FragmentShader::~FragmentShader() {

}