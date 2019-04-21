#pragma once

#include "../Tools/MeanLeanWindows.h"

namespace Discrepancy
{
	class SoundEngine
	{
		void *_buffer;
	public:
		// duration seconds
		SoundEngine(float duration);
		~SoundEngine();

		void Generate();
		void Play();
	};
}