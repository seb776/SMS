//#include <GL\glew.h>
#include "MeanLeanWindows.h"
#include "GLHelper.h"
#include ".\..\dependencies\gl\glext.h"
#include "FragmentShader.h"
#include "Memory.h"

FragmentShader::FragmentShader() {

}


bool FragmentShader::Load(const char *path, bool isPath) {
	if (isPath)
	{
		_OFSTRUCT lpof;

		HFILE hfile = OpenFile(path, &lpof, OF_READ);

		DWORD bytes_read = 0;
		ReadFile((HANDLE)hfile, &_shaderCode, lpof.cBytes, &bytes_read, nullptr);
		_shaderCode[bytes_read] = '\0';
		CloseHandle((HANDLE)hfile);
	}
	else
	{
		Memory::MemCpy(_shaderCode, path, 2048);
		_shaderCode[2047] = '\0';
	}

	return true;
}



unsigned int FragmentShader::Run() {
	
	const unsigned int t = glCreateShaderProgramEXT(GL_FRAGMENT_SHADER, _shaderCode);
	
	GLint status = 0;
	glGetShaderiv(t, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLenum errCode;
		const GLubyte *errString;
		if ((errCode = glGetError()) !=
			GL_NO_ERROR)
		{
			errString = glGetString(errCode);
			OutputDebugString((char*)errString);
		}
		//return 0;
	}

	status = 0;
	glGetShaderiv(t, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLenum errCode;
		const GLubyte *errString;
		if ((errCode = glGetError()) !=
			GL_NO_ERROR)
		{
			errString = glGetString(errCode);
			OutputDebugString((char*)errString);
		}
		//return 0;
	}
	glUseProgram(t);
	return t;
}


FragmentShader::~FragmentShader() {

}