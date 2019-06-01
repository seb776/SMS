#pragma once

#include <Tools/MeanLeanWindows.h>

namespace Discrepancy
{
	namespace Synthesizer
	{
		struct WaveHeader
		{
			DWORD chunkID;       // 0x46464952 "RIFF" in little endian
			DWORD chunkSize;     // 4 + (8 + subChunk1Size) + (8 + subChunk2Size)
			DWORD format;        // 0x45564157 "WAVE" in little endian

			DWORD subChunk1ID;   // 0x20746d66 "fmt " in little endian
			DWORD subChunk1Size; // 16 for PCM
			WORD  audioFormat;   // 1 for PCM, 3 fot EEE floating point , 7 for μ-law
			WORD  numChannels;   // 1 for mono, 2 for stereo
			DWORD sampleRate;    // 8000, 22050, 44100, etc...
			DWORD byteRate;      // sampleRate * numChannels * bitsPerSample/8
			WORD  blockAlign;    // numChannels * bitsPerSample/8
			WORD  bitsPerSample; // number of bits (8 for 8 bits, etc...)

			DWORD subChunk2ID;   // 0x61746164 "data" in little endian
			DWORD subChunk2Size; // numSamples * numChannels * bitsPerSample/8 (this is the actual data size in bytes)
		};

		class WaveUtils
		{
		public:
			// does not take WaveHeader into account
			static int CalculateBufferSize(int bps, int numCh, int sampleRate, float duration);
			// bps = bit per sample
			// numCh = numChannels
			// 
			static void GenerateWaveHeader(void *soundBuf, unsigned int bufferSize, int bps, int numCh, int sampleRate);
		};
	}
}