#pragma once

#include "MeanLeanWindows.h"
#include <GL\gl.h>
#include <GL\glu.h>
#include <gl\glext.h>
#include <MMSystem.h>


#define DEFINE_GL_FUNC(type, name) type name;
#define DECLARE_GL_FUNC(type, name) extern type name;

DECLARE_GL_FUNC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
DECLARE_GL_FUNC(PFNGLUNIFORM1FPROC, glUniform1f);
DECLARE_GL_FUNC(PFNGLUNIFORM2FPROC, glUniform2f);
DECLARE_GL_FUNC(PFNGLCREATESHADERPROGRAMEXTPROC, glCreateShaderProgramEXT);
DECLARE_GL_FUNC(PFNGLGETSHADERIVPROC, glGetShaderiv);
DECLARE_GL_FUNC(PFNGLUSEPROGRAMPROC, glUseProgram);
DECLARE_GL_FUNC(PFNGLLINKPROGRAMPROC, glLinkProgram);

//glGetShaderInfoLog

int initOpenGL();