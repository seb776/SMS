#include <GL\glew.h>
#include "FragmentShader.h"
#include "MeanLeanWindows.h"
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



int FragmentShader::Run() {
	const int t = ((PFNGLCREATESHADERPROGRAMEXTPROC)wglGetProcAddress("glCreateShaderProgramEXT"))(GL_FRAGMENT_SHADER, _shaderCode);
	((PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram"))(t);
	return t;
}


FragmentShader::~FragmentShader() {

}