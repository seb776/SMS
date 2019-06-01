#pragma once

#include <Tools/Math.h>

namespace Discrepancy
{
	namespace Synthesizer
	{
		class ADSREnvelope
		{
		private:
			float _onTime;

			float _attackTime;
			float _decayTime;
			float _sustainTime;
			float _releaseTime;

			float _attackAmplitude;
			float _sustainAmplitude;

		public:
			ADSREnvelope(float onTime, float attackTime, float decayTime, float sustainTime, float releaseTime, float attackAmplitude, float sustainAmplitude);
			float GetAmplitude(float time);
		};
	}
}