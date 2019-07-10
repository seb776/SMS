#include <Tools/GLHelper.h>
#include <dependencies/gl/glext.h>
#include <Tools/MeanLeanWindows.h>
#include <Tools/Memory.h>
#include "FragmentShader.h"

using namespace Discrepancy;

FragmentShader::FragmentShader(GLuint frameBufferName) {

}

int mystrlen(const char *buf)
{
	int i = 0;
	for (i = 0; buf[i] != '\0'; ++i)
		;
	return i;
}

int FragmentShader::Load(const char code[]) {
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
		return -1;
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
		return -1;
	}

	_timeLocation = glGetUniformLocation((GLuint)_programId, "time");
	glUniform1f(_timeLocation, 0.0f);

	_resolutionLocation = glGetUniformLocation((GLuint)_programId, "resolution");
	glUniform2f(_resolutionLocation, (float)1920, (float)1080); // TODO
	return _shaderId;
}


void FragmentShader::Render(ShaderParameters parameters, GLuint renderTexture, GLuint frameBufferName)
{
	 // TODO handle parameters passing and texture exchange
	glUseProgram(_programId);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glRects(-1, -1, 1, 1);
}


FragmentShader::~FragmentShader() {

}