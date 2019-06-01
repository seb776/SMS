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

			void Generate(float duration);
			void Play();
			void Stop();
		};
	}
}