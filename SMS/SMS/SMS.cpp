// SMS.cpp : Defines the entry point for the application.
//

//#define FINAL_COMPO

#include "targetver.h"
#include <GL\glew.h>
#include "Tools/MeanLeanWindows.h"
#include "SMS.h"
#include <io.h>
#include "Window\ParametersWindow.h"


#define SMS_ERROR_MSG "ERROR"

// Run in windowed mode in debug but maximized in release.
#ifndef _DEBUG
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP | WS_MAXIMIZE)
#else
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP)
#endif

const static PIXELFORMATDESCRIPTOR pfd = { 0, 0, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const static char *fragmentShader = \
//"varying vec4 v;"
"void main()"
"{"
"vec2 v=-1.+gl_FragCoord.xy/vec2(640.,360.);"
// tunnel
"float r=pow(pow(abs(v.x),18.)+pow(abs(v.y),10.),1.8/28.);"
"vec2 t=vec2(gl_Color.x*64.+1./r,atan(v.x,v.y));"
// texture
"t=fract(2*t)-.5;"
"vec4 col=(1-pow(dot(t.xy,t.xy),.3))*vec4(2,1.8,1.6,0)+vec4(.3,.2,.1,0)+.12*vec4(v,0,0);"
// final color
"gl_FragColor=col/(2.*r);"
"}";

void CreateOpenGLFullScreen(int width, int height)
{
	//if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) return -1;
	ShowCursor(0);
	HDC hDC = GetDC(CreateWindowExA(WS_EX_APPWINDOW, "static", 0, GL_WINDOWFLAGS, 0, 0, width, height, 0, 0, 0, 0));
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglMakeCurrent(hDC, wglCreateContext(hDC));

	const int t = ((PFNGLCREATESHADERPROGRAMEXTPROC)wglGetProcAddress("glCreateShaderProgramEXT"))(GL_FRAGMENT_SHADER, fragmentShader);
	((PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram"))(t);
	do
	{
		static MSG dummyMessage;
		PeekMessageA(&dummyMessage, 0, 0, 0, 1); // Remove all Windows messages to prevent "Program not responding" dialog.
		glColor3us((unsigned short)GetTickCount(), 0, 0);
		glRects(-1, -1, 1, 1);
		SwapBuffers(hDC); //wglSwapLayerBuffers( hDC, WGL_SWAP_MAIN_PLANE );
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
