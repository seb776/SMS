#include <Tools/MeanLeanWindows.h>
#include <MMSystem.h>

#include <Tools/Memory.h>
#include <Tools/Math.h>
#include <Tools/DebugTools/Debug.h>

#include "SoundEngine.h"
#include "Models/WaveHeader.h"
#include "Envelopes/ADSREnvelope.h"

using namespace Discrepancy;
using namespace Discrepancy::Synthesizer;

// Useful links
// https://docs.microsoft.com/en-us/previous-versions//dd743863(v=vs.85)
// Sound https://stackoverflow.com/questions/1451606/programably-make-and-play-a-sound-through-speakers-c
// http://soundfile.sapp.org/doc/WaveFormat/
// https://www.youtube.com/watch?v=tgamhuQnOkM
//

Discrepancy::Synthesizer::SoundEngine::SoundEngine() :
	_buffer(nullptr)
{

}

Discrepancy::Synthesizer::SoundEngine::~SoundEngine()
{
	if (_buffer != nullptr)
		Memory::HeapFree(_buffer);
}

void Discrepancy::Synthesizer::SoundEngine::Generate(float duration)
{
	int bps = 8;
	int sampleRate = 44100;
	int channels = 2;

	int bufferSize = WaveUtils::CalculateBufferSize(bps, channels, sampleRate, duration);

	_buffer = Memory::HeapAlloc(bufferSize + sizeof(WaveHeader));
	Memory::Memset((char*)_buffer, bufferSize);

	WaveUtils::GenerateWaveHeader(_buffer, bufferSize, bps, channels, sampleRate);


	WaveHeader *header = (WaveHeader*)_buffer;
	unsigned char *data = (unsigned char *)_buffer + sizeof(WaveHeader);

	char nbr[16];

	float frequency = 440.0f;
	int sampleSize = (bps / 8)*channels;
	float volume = 0.7f;
	Synthesizer::ADSREnvelope envelope = Synthesizer::ADSREnvelope(0.0f, 0.05f, 0.05f, 0.1f, 0.05f, 1.0f, 0.7f);
		for (int iSample = 0; iSample < (bufferSize - sizeof(WaveHeader)); iSample += sampleSize)
		{
			float seconds = (float)iSample / ((float)sampleRate * (float)(bps / 8) * channels);

			if ((iSample % (sampleRate)) == 0 && seconds < 4.0f)
				frequency += 25.0f;


			int right = (int)(((Math::Sin(seconds * frequency * MathConstants::PI * 2.0f) * 0.5f) + 0.5f) * 255.0f);//  *envelope.GetAmplitude(seconds) * volume);
			int left = left;


			//int left = (int)((Math::Fmod(seconds * frequency, 1.0f))*255.0f * envelope.GetAmplitude(seconds) * volume);
			//int right = left;

			switch (bps)
			{
			case 8:
				data[iSample] = left;
				data[iSample + 1] = right;
			default:
				break;
			}

			if ((iSample % 100) == 0)
			{
				float percent = ((float)iSample / (float)bufferSize)*100.0f;
				int pct = (int)percent;

				Memory::Memset(nbr, 16);
				OutputDebugString(putnbr(pct, nbr, 15));
				OutputDebugStringW(L"/");
				Memory::Memset(nbr, 16);
				OutputDebugString(putnbr(100, nbr, 15));
				OutputDebugStringW(L"\n");
			}
		}


}

void Discrepancy::Synthesizer::SoundEngine::Play()
{
	LPCSTR soundBuf = (LPCSTR)_buffer;

	
	PlaySound(soundBuf, NULL, SND_MEMORY | SND_ASYNC);
}

void Discrepancy::Synthesizer::SoundEngine::Stop()
{
	PlaySound(NULL, NULL, SND_MEMORY | SND_ASYNC);
}
