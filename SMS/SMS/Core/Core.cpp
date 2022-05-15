#include <Tools/MeanLeanWindows.h>
#include <Composition/VisualComposition.h>

#include <SoundEngine/SoundEngine.h>
#include <Tools/FragmentShader.h>
#include <ShaderBuilt/ShaderBuilt.h>
#include <SoundEngine/Envelopes/ADSREnvelope.h>
#include <Tools/GLHelper.h>
#include <Tools/GDIHelper.h>
#include <new>

#include "Core.h"

using namespace Discrepancy;
using namespace Composition;

static bool g_finished = false;
static float g_progress = 0.;

static float g_width = 0.;
static float g_height = 0.;

void runLoadingScreen(HDC hDC)
{
	float accTime = 0.;
	FragmentShader loadingShader(0);

	int shaderIdx = loadingShader.Load(Loading);

	loadingShader.Render(ShaderParameters(), 0);

	GLint myTimeLocation = glGetUniformLocation((GLuint)shaderIdx, "time");
	GLint myResLocation = glGetUniformLocation((GLuint)shaderIdx, "resolution");
	GLint myProgressLocation = glGetUniformLocation((GLuint)shaderIdx, "progress");



	float elapsedLastFrame = 0.;
	while (!g_finished)
	{
		DWORD lastFrame = GetTickCount();

		static MSG dummyMessage;
		PeekMessageA(&dummyMessage, 0, 0, 0, 1); // Remove all Windows messages to prevent "Program not responding" dialog.

		glUniform1f(myProgressLocation, g_progress);
		glUniform1f(myTimeLocation, accTime);
		glUniform2f(myResLocation, g_width, g_height);



		glRects(-1, -1, 1, 1);
		SwapBuffers(hDC);
		elapsedLastFrame = (float)(GetTickCount() - lastFrame) / 1000.0f;
		accTime += elapsedLastFrame;
	}
}

DWORD WINAPI threadLoading(LPVOID lpParameter)
{
	auto soundEngine = (Synthesizer::SoundEngine*)lpParameter;
	soundEngine->Generate(4.0, &g_progress);
	g_finished = true;
	return 0;
}


VisualComposition *GenerateVisualCompo()
{
	VisualComposition *visualCompo = (decltype(visualCompo))Memory::HeapAlloc(sizeof(*visualCompo));

	visualCompo->Init(5);
	//glFramebufferTexture()

	auto channelsContent = visualCompo->GetChannelsContent();

	auto shaderLoading = NEW(FragmentShader)(0);
	auto frameLoading = NEW(VisualFrame)(shaderLoading, 0.f, -1.0f, visualCompo->FrameBufferName);

	shaderLoading->Load(Loading);

	//frame->Content
	//channelsContent[0].Frames.Push()

	return visualCompo;
}


void TestVisualCompositionRender()
{
	VisualComposition *visualCompo = GenerateVisualCompo();
	float accTime = 0.f;





	visualCompo->Lock();
	while (false)
	{
		DWORD lastFrame = GetTickCount();
		visualCompo->ExecuteComposition(0.f);
		float elapsedLastFrame = (float)(GetTickCount() - lastFrame) / 1000.0f;
		accTime += elapsedLastFrame;
	}
	Memory::HeapFree(visualCompo);
}

void Core::RunOpenGLInWindow(const StartParameters& startParams)
{

	ShowCursor(0);
	HDC hDC = GetDC(CreateWindowExA(WS_EX_APPWINDOW, "static", 0, startParams.WindowFlags, 0, 0, startParams.Width, startParams.Height, 0, 0, 0, 0));
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

	wglMakeCurrent(hDC, wglCreateContext(hDC));

	initOpenGL();

	glEnable(GL_TEXTURE_2D);

	//TestVisualCompositionRender();
	//return;

	Synthesizer::SoundEngine soundEngine;
	DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, threadLoading, &soundEngine, 0, &myThreadID);

	g_width = startParams.Width;
	g_height = startParams.Height;


	runLoadingScreen(hDC);


	soundEngine.Play();

	FragmentShader overlay(0);

	overlay.Load(TestOverlay);

	FragmentShader fs(0);

	int shaderIdx = fs.Load(DebugMathFunction);

	fs.Render(ShaderParameters(), 0);
	//unsigned int shaderIdx = overlay.Run();

	Synthesizer::ADSREnvelope envelope = Synthesizer::ADSREnvelope(1.0f, 0.1f, 0.1f, 1.0f, 0.1f, 1.0f, 0.7f);

	float accTime = 0.0f;
	float elapsedLastFrame = 0.0f;
	const int valuesCnt = 128 * 2;
	float values[valuesCnt];
	float valuesCos[valuesCnt];
	float it = -10.0;
	float max = -it;
	float step = 2.0*(max - it) / (float)valuesCnt;
	for (int i = 0; i < valuesCnt; i += 2)
	{

		values[i] = it;
		values[i + 1] = Math::Sin(it);
		valuesCos[i] = it * 0.2f + 1.0f;
		valuesCos[i + 1] = envelope.GetAmplitude(valuesCos[i]);
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
		glUniform2f(myResLocation, (float)startParams.Width, (float)startParams.Height);


		glRects(-1, -1, 1, 1);
		SwapBuffers(hDC);
		elapsedLastFrame = (float)(GetTickCount() - lastFrame) / 1000.0f;
		accTime += elapsedLastFrame;
	} while (!GetAsyncKeyState(VK_ESCAPE));
	ExitProcess(0);

	soundEngine.Stop();
}