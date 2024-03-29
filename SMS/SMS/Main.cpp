#include "Main.h"
#include "Window\ParametersWindow.h"
#include "Tools\FragmentShader.h"
#include "ShaderBuilt\ShaderBuilt.h"
#include <Core/Core.h>
#include <Tools/GLHelper.h>
#include <Tools/DebugTools/Debug.h>

using namespace Discrepancy;

#ifndef DEBUG
static const bool GET_START_PARAMETERS_FROM_UI = true;
#else
static const bool GET_START_PARAMETERS_FROM_UI = false;
#endif


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const auto& startParams = Core::GetStartParameters(GET_START_PARAMETERS_FROM_UI, lpCmdLine, nCmdShow);

#ifdef DEBUG
	ParametersWindow paramWindow = ParametersWindow(hInstance);

	paramWindow.StartWindow();
	int ret = paramWindow.MessageLoop();
	return ret;
#else
	Core::RunOpenGLInWindow(startParams);
#endif FINAL_COMPO

}