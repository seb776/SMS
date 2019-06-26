#include <Tools/GDIHelper.h>
#include "StartParameters.h"

using namespace Discrepancy;

// Run in windowed mode in debug but maximized in release.
#ifndef DEBUG
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP | WS_MAXIMIZE)
#else
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP)
#endif

Core::StartParameters Core::GetStartParameters(bool fromUI, LPSTR lpCmdLine, int nCmdShow)
{
	auto startParams = StartParameters();

	startParams.FramePerSecond = 60.0f;
	startParams.Width = 1920U;
	startParams.Height = 1080U;

	startParams.WindowFlags = GL_WINDOWFLAGS;
	return startParams;
}