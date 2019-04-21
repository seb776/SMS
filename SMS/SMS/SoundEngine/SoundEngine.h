#pragma once

#include "../Tools/MeanLeanWindows.h"

namespace Discrepancy
{
	class SoundEngine
	{
		void *_buffer;
	public:
		// duration seconds
		SoundEngine();
		~SoundEngine();

		void Generate(float duration);
		void Play();
	};
}