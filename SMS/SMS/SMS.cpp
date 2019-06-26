// SMS.cpp : Defines the entry point for the application.
//

//#include "targetver.h"
#include "Tools/MeanLeanWindows.h"
#include <GL\gl.h>
#include <GL\glu.h>
//#include <gl\glext.h>
#include "SMS.h"
#include "Window\ParametersWindow.h"
#include "Tools\FragmentShader.h"
#include "ShaderBuilt\ShaderBuilt.h"

#include <Tools/DebugTools/Debug.h>
#include <Tools/Math.h>
#include <Tools/GLHelper.h>

#include <SoundEngine/SoundEngine.h>
#include <SoundEngine/Envelopes/ADSREnvelope.h>

#include <Core.h>

using namespace Discrepancy;

// Run in windowed mode in debug but maximized in release.
#ifndef DEBUG
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP | WS_MAXIMIZE)
#else
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP)
#endif

const static PIXELFORMATDESCRIPTOR pfd = { 0, 0, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DIRECT3D_ACCELERATED, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	initOpenGL();

#ifdef DEBUG
	ParametersWindow paramWindow = ParametersWindow(hInstance);

	paramWindow.StartWindow();
	int ret = paramWindow.MessageLoop();
	return ret;
#else
	Core::CreateOpenGLFullScreen(1280, 720);
#endif FINAL_COMPO

}