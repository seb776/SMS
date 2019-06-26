#pragma once

#include <Tools/MeanLeanWindows.h>

namespace Discrepancy
{
	namespace Core
	{
		struct StartParameters
		{
			unsigned int Width;
			unsigned int Height;

			float FramePerSecond;

			DWORD WindowFlags;
		};

		StartParameters GetStartParameters(bool fromUI, LPSTR lpCmdLine, int nCmdShow);
	}
}