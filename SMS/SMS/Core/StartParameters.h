#pragma once

#include <Tools/MeanLeanWindows.h>

namespace Discrepancy
{
	namespace Core
	{
		struct StartParameters
		{
			StartParameters() :
				Width(1920), Height(1080), FramePerSecond(60.0f), WindowFlags(0)
			{}

			unsigned int Width;
			unsigned int Height;

			float FramePerSecond;

			DWORD WindowFlags;
		};

		StartParameters GetStartParameters(bool fromUI, LPSTR lpCmdLine, int nCmdShow);
	}
}