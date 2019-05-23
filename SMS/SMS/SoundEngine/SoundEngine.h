#pragma once

#include "../Tools/MeanLeanWindows.h"

namespace Discrepancy
{
	class SoundEngine
	{
		void *_buffer;
	public:
		SoundEngine();
		~SoundEngine();

		void Generate(float duration);
		void Play();
	};
}