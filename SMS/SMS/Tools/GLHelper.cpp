
#include "GLHelper.h"

DEFINE_GL_FUNC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)
DEFINE_GL_FUNC(PFNGLUNIFORM1FPROC, glUniform1f)
DEFINE_GL_FUNC(PFNGLUNIFORM2FPROC, glUniform2f)
DEFINE_GL_FUNC(PFNGLCREATESHADERPROGRAMEXTPROC, glCreateShaderProgramEXT)
DEFINE_GL_FUNC(PFNGLGETSHADERIVPROC, glGetShaderiv)
DEFINE_GL_FUNC(PFNGLUSEPROGRAMPROC, glUseProgram)
DEFINE_GL_FUNC(PFNGLLINKPROGRAMPROC, glLinkProgram)
DEFINE_GL_FUNC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)
DEFINE_GL_FUNC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)

DEFINE_GL_FUNC(PFNGLCREATESHADERPROC, glCreateShader)
DEFINE_GL_FUNC(PFNGLSHADERSOURCEPROC, glShaderSource)
DEFINE_GL_FUNC(PFNGLCOMPILESHADERPROC, glCompileShader)

DEFINE_GL_FUNC(PFNGLCREATEPROGRAMPROC, glCreateProgram)
DEFINE_GL_FUNC(PFNGLATTACHSHADERPROC, glAttachShader)
DEFINE_GL_FUNC(PFNGLGETPROGRAMIVPROC, glGetProgramiv)



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
	INIT_GL_FUNC(glGetShaderInfoLog);
	INIT_GL_FUNC(glGetProgramInfoLog);

	INIT_GL_FUNC(glCreateShader);
	INIT_GL_FUNC(glShaderSource);
	INIT_GL_FUNC(glCompileShader);

	INIT_GL_FUNC(glCreateProgram);
	INIT_GL_FUNC(glAttachShader);
	INIT_GL_FUNC(glGetProgramiv);

	return 0;
}