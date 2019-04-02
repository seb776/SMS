// SMS.cpp : Defines the entry point for the application.
//

//#include "targetver.h"
#include "Tools/MeanLeanWindows.h"
#include <GL\gl.h>
#include <GL\glu.h>
#include <gl\glext.h>
#include <MMSystem.h>
#include "SMS.h"
#include "Window\ParametersWindow.h"
#include "Tools\FragmentShader.h"
#include "ShaderBuilt\ShaderBuilt.h"
#include "Tools\Debug.h"
#include "Tools\Math.h"
#include "Tools\GLHelper.h"



// Run in windowed mode in debug but maximized in release.
#ifndef DEBUG
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP | WS_MAXIMIZE)
#else
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP)
#endif

const static PIXELFORMATDESCRIPTOR pfd = { 0, 0, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DIRECT3D_ACCELERATED, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


struct WaveHeader
{
	DWORD chunkID;       // 0x46464952 "RIFF" in little endian
	DWORD chunkSize;     // 4 + (8 + subChunk1Size) + (8 + subChunk2Size)
	DWORD format;        // 0x45564157 "WAVE" in little endian

	DWORD subChunk1ID;   // 0x20746d66 "fmt " in little endian
	DWORD subChunk1Size; // 16 for PCM
	WORD  audioFormat;   // 1 for PCM, 3 fot EEE floating point , 7 for μ-law
	WORD  numChannels;   // 1 for mono, 2 for stereo
	DWORD sampleRate;    // 8000, 22050, 44100, etc...
	DWORD byteRate;      // sampleRate * numChannels * bitsPerSample/8
	WORD  blockAlign;    // numChannels * bitsPerSample/8
	WORD  bitsPerSample; // number of bits (8 for 8 bits, etc...)

	DWORD subChunk2ID;   // 0x61746164 "data" in little endian
	DWORD subChunk2Size; // numSamples * numChannels * bitsPerSample/8 (this is the actual data size in bytes)
};

#define SOUND_BUFFER_SIZE (8000 * 8)



void CreateOpenGLFullScreen(int width, int height)
{
	//if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) return -1;
	ShowCursor(0);
	HDC hDC = GetDC(CreateWindowExA(WS_EX_APPWINDOW, "static", 0, GL_WINDOWFLAGS, 0, 0, width, height, 0, 0, 0, 0));
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglCreateContext(hDC);
	wglMakeCurrent(hDC, wglCreateContext(hDC));
	initOpenGL();

	//glewInit();

	LPVOID soundMemPtr = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY/*HEAP_GENERATE_EXCEPTIONS*/, SOUND_BUFFER_SIZE);
	LPCSTR soundBuf = (LPCSTR)soundMemPtr;

	// TODO
	// Sound https://stackoverflow.com/questions/1451606/programably-make-and-play-a-sound-through-speakers-c
	// http://soundfile.sapp.org/doc/WaveFormat/

	WaveHeader *header = (WaveHeader*)soundBuf;
	// fill out the header...

	char *data = (char*)soundBuf + sizeof(WaveHeader); //jumps to beginning of data

	header->chunkID = 0x46464952;       // 0x46464952 "RIFF" in little endian
	header->chunkSize = 4 + (8 + 16) + (8 + SOUND_BUFFER_SIZE - sizeof(WaveHeader));     // 4 + (8 + subChunk1Size) + (8 + subChunk2Size)
	header->format = 0x45564157;        // 0x45564157 "WAVE" in little endian

	header->subChunk1ID = 0x20746d66;   // 0x20746d66 "fmt " in little endian
	header->subChunk1Size = 16; // 16 for PCM
	header->audioFormat = 1;   // 1 for PCM, 3 fot EEE floating point , 7 for μ-law
	header->numChannels = 1;   // 1 for mono, 2 for stereo
	header->sampleRate = 8000;    // 8000, 22050, 44100, etc...
	header->byteRate = 8000 * 1 * 8 / 8;      // sampleRate * numChannels * bitsPerSample/8
	header->blockAlign = 1;    // numChannels * bitsPerSample/8
	header->bitsPerSample = 8; // number of bits (8 for 8 bits, etc...)

	header->subChunk2ID = 0x61746164;   // 0x61746164 "data" in little endian
	header->subChunk2Size = SOUND_BUFFER_SIZE - sizeof(WaveHeader);// 7000; // numSamples * numChannels * bitsPerSample/8 (this is the actual data size in bytes)

	
	for (int i = sizeof(WaveHeader) ; i < SOUND_BUFFER_SIZE; ++i)
	{
		((unsigned char*)(soundBuf))[i] = i % 200;// (unsigned char)(MySin(((float)i / (float)SOUND_BUFFER_SIZE)*440.0 / 8000.0) * 127.0f) + 126U;
	}

	PlaySound(soundBuf, NULL, SND_MEMORY | SND_ASYNC);
	FragmentShader fs;

	fs.Load(RetroReflect);

	unsigned int shaderIdx = fs.Run();


	float accTime = 0.0f;
	float elapsedLastFrame = 0.0f;
	do
	{
		DWORD lastFrame = GetTickCount();
		static MSG dummyMessage;
		PeekMessageA(&dummyMessage, 0, 0, 0, 1); // Remove all Windows messages to prevent "Program not responding" dialog.
		//glColor3us((unsigned short)GetTickCount(), 0, 0);

		GLint myUniformLocation = glGetUniformLocation((GLuint)shaderIdx, "iGlobalTime");
		glUniform1f(0, accTime);

		GLint myResLocation = glGetUniformLocation((GLuint)shaderIdx, "iResolution");
		glUniform2f(1, (float)width, (float)height);


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