// SMS.cpp : Defines the entry point for the application.
//

//#include "targetver.h"
#include "Tools/MeanLeanWindows.h"
#include <GL\gl.h>
#include <GL\glu.h>
#include <gl\glext.h>
#include "SMS.h"
#include "Window\ParametersWindow.h"
#include "Tools\FragmentShader.h"
#include "ShaderBuilt\ShaderBuilt.h"
#include "Tools\Debug.h"
#include "Tools\Math.h"
#include "Tools\GLHelper.h"
#include "SoundEngine/SoundEngine.h"

using namespace Discrepancy;

// Run in windowed mode in debug but maximized in release.
#ifndef DEBUG
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP | WS_MAXIMIZE)
#else
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP)
#endif

const static PIXELFORMATDESCRIPTOR pfd = { 0, 0, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DIRECT3D_ACCELERATED, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };







void CreateOpenGLFullScreen(int width, int height)
{
	//if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) return -1;
	ShowCursor(0);
	HDC hDC = GetDC(CreateWindowExA(WS_EX_APPWINDOW, "static", 0, GL_WINDOWFLAGS, 0, 0, width, height, 0, 0, 0, 0));
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglCreateContext(hDC);
	wglMakeCurrent(hDC, wglCreateContext(hDC));

	initOpenGL();

	SoundEngine soundEngine;

	soundEngine.Generate(10.0);
	soundEngine.Play();

	FragmentShader fs;

	fs.Load(DebugMathFunction);


	unsigned int shaderIdx = fs.Run();

	float accTime = 0.0f;
	float elapsedLastFrame = 0.0f;
	const int valuesCnt = 128 * 2;
	float values[valuesCnt];
	float valuesCos[valuesCnt];
	float it = -10.0;
	float max = -it;
	float step = 2.0*(max - it) / (float)valuesCnt;
	for (int i = 0; i < valuesCnt; i+=2)
	{
		
		values[i] = it;
		values[i + 1] = Math::Sin(it);
		valuesCos[i] = it;
		valuesCos[i + 1] = Math::Cos(it);
		it += step;
	}

	GLint curveCnt = glGetUniformLocation((GLuint)shaderIdx, "CURVE_CNT");
	glUniform1i(curveCnt, 2);

	GLint curveLocation = glGetUniformLocation((GLuint)shaderIdx, "curveA");
	glUniform2fv(curveLocation, 128, (const GLfloat *)&values);

	curveLocation = glGetUniformLocation((GLuint)shaderIdx, "curveB");
	glUniform2fv(curveLocation, 128, (const GLfloat *)&valuesCos);


	do
	{
		DWORD lastFrame = GetTickCount();
		static MSG dummyMessage;
		PeekMessageA(&dummyMessage, 0, 0, 0, 1); // Remove all Windows messages to prevent "Program not responding" dialog.
		glColor3us((unsigned short)GetTickCount(), 0, 0);

		GLint myUniformLocation = glGetUniformLocation((GLuint)shaderIdx, "time");
		glUniform1f(myUniformLocation, accTime);

		GLint myResLocation = glGetUniformLocation((GLuint)shaderIdx, "resolution");
		glUniform2f(myResLocation, (float)width, (float)height);


		glRects(-1, -1, 1, 1);
		SwapBuffers(hDC); // wglSwapLayerBuffers( hDC, WGL_SWAP_MAIN_PLANE );
		elapsedLastFrame = (float)(GetTickCount() - lastFrame) / 1000.0f;
		accTime += elapsedLastFrame;
	} while (!GetAsyncKeyState(VK_ESCAPE));
	ExitProcess(0);
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	LogConsole("Hey");
	int i = 0;

	initOpenGL();

#ifndef FINAL_COMPO
	ParametersWindow paramWindow = ParametersWindow(hInstance);

	paramWindow.StartWindow();
	int ret = paramWindow.MessageLoop();
	return ret;
#else
	CreateOpenGLFullScreen(1280, 720);
#endif FINAL_COMPO

}