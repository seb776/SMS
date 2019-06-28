#pragma once

#include "../Tools/MeanLeanWindows.h"

namespace Discrepancy
{
	namespace Synthesizer
	{
		class SoundEngine
		{
			void *_buffer;
		public:
			SoundEngine();
			~SoundEngine();

			void Generate(float duration, float *progress);
			void Play();
			void Stop();
		};
	}
}