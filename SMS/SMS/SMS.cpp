// SMS.cpp : Defines the entry point for the application.
//

//#define FINAL_COMPO

#include "targetver.h"
#define GLEW_STATIC
#include <GL\glew.h>
#include "Tools/MeanLeanWindows.h"
#include "SMS.h"
#include <io.h>
#include "Window\ParametersWindow.h"
#include "Tools\FragmentShader.h"
#include "ShaderBuilt\ShaderBuilt.h"

// To allow floating point operations
extern "C" {
	int _fltused = 1;
}


#define SMS_ERROR_MSG "ERROR"

// Run in windowed mode in debug but maximized in release.
#ifndef _DEBUG
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP | WS_MAXIMIZE)
#else
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP)
#endif

const static PIXELFORMATDESCRIPTOR pfd = { 0, 0, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void CreateOpenGLFullScreen(int width, int height)
{
	//if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) return -1;
	ShowCursor(0);
	HDC hDC = GetDC(CreateWindowExA(WS_EX_APPWINDOW, "static", 0, GL_WINDOWFLAGS, 0, 0, width, height, 0, 0, 0, 0));
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglMakeCurrent(hDC, wglCreateContext(hDC));


	FragmentShader fs;

	fs.Load(RetroReflect);

	int shaderIdx = fs.Run();

	float accTime = 0.0f;
	float elapsedLastFrame = 0.0f;
	do
	{
		DWORD lastFrame = GetTickCount();
		static MSG dummyMessage;
		PeekMessageA(&dummyMessage, 0, 0, 0, 1); // Remove all Windows messages to prevent "Program not responding" dialog.
		

		glColor3us((unsigned short)GetTickCount(), 0, 0);
		//GLint myUniformLocation = glGetUniformLocation((GLuint)shaderIdx, "iGlobalTime");
		//glUniform1f(myUniformLocation, accTime);

		glRects(-1, -1, 1, 1);
		SwapBuffers(hDC); //wglSwapLayerBuffers( hDC, WGL_SWAP_MAIN_PLANE );
		elapsedLastFrame = (float)(GetTickCount() - lastFrame) / 1000.0f;
		accTime += elapsedLastFrame;
	} while (!GetAsyncKeyState(VK_ESCAPE));
	ExitProcess(0);
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
#ifndef FINAL_COMPO
	ParametersWindow paramWindow = ParametersWindow(hInstance);

	paramWindow.StartWindow();
	int ret = paramWindow.MessageLoop();
	return ret;
#else
	CreateOpenGLFullScreen(1280, 720);
#endif FINAL_COMPO

}
