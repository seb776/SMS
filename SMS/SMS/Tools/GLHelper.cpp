
#include "GLHelper.h"

DEFINE_GL_FUNC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)
DEFINE_GL_FUNC(PFNGLUNIFORM1FPROC, glUniform1f)
DEFINE_GL_FUNC(PFNGLUNIFORM2FPROC, glUniform2f)
DEFINE_GL_FUNC(PFNGLCREATESHADERPROGRAMEXTPROC, glCreateShaderProgramEXT)
DEFINE_GL_FUNC(PFNGLGETSHADERIVPROC, glGetShaderiv)
DEFINE_GL_FUNC(PFNGLUSEPROGRAMPROC, glUseProgram)
DEFINE_GL_FUNC(PFNGLLINKPROGRAMPROC, glLinkProgram)


static void *GetAnyGLFuncAddress(const char *name)
{
	void *p = (void *)wglGetProcAddress(name);
	if (p == 0 ||
		(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
		(p == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}

	return p;
}
// We put it there because the function it calls is only defined in this compilation unit
#define INIT_GL_FUNC(name) { name = (decltype(name))GetAnyGLFuncAddress(#name);	}

int initOpenGL()
{
	INIT_GL_FUNC(glGetUniformLocation);
	INIT_GL_FUNC(glUniform1f);
	INIT_GL_FUNC(glUniform2f);
	INIT_GL_FUNC(glCreateShaderProgramEXT);
	INIT_GL_FUNC(glGetShaderiv);
	INIT_GL_FUNC(glUseProgram);
	INIT_GL_FUNC(glLinkProgram);
	return 0;
}