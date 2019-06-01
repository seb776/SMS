#include "ADSREnvelope.h"

Discrepancy::Synthesizer::ADSREnvelope::ADSREnvelope(float onTime, float attackTime, float decayTime, float sustainTime, float releaseTime, float attackAmplitude, float sustainAmplitude) :
	_onTime(onTime), _attackTime(attackTime), _decayTime(decayTime), _sustainTime(sustainTime), _releaseTime(releaseTime),
	_attackAmplitude(attackAmplitude), _sustainAmplitude(sustainAmplitude)
{
}

float Discrepancy::Synthesizer::ADSREnvelope::GetAmplitude(float time)
{
	float amplitude = 0.0f;
	float curTime = time - _onTime;
	if (curTime > -MathConstants::EPS)
	{
		if (curTime < _attackTime)
			amplitude = Math::Lerp(0.f, _attackAmplitude, curTime / _attackTime);
		else if (curTime < (_attackTime + _decayTime))
			amplitude = Math::Lerp(_attackAmplitude, _sustainAmplitude, (curTime - _attackTime) / _decayTime);
		else if (curTime < (_attackTime + _decayTime + _sustainTime))
			amplitude = _sustainAmplitude;
		else if (curTime < (_attackTime + _decayTime + _sustainTime + _releaseTime))
			amplitude = Math::Lerp(_sustainAmplitude, 0.0f, (curTime - _attackTime - _decayTime - _sustainTime) / _decayTime);
		else
			amplitude = 0.0f;
	}

	return amplitude;
}