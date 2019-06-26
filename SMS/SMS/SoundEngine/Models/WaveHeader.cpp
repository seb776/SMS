#include <Tools/Math.h>
#include "WaveHeader.h"

int Discrepancy::Synthesizer::WaveUtils::CalculateBufferSize(int bps, int numCh, int sampleRate, float duration)
{
	int bufferSize = sizeof(WaveHeader) +						// Header
		numCh * (bps / 8)*sampleRate*(int)Math::Ceil(duration); // Data
	return bufferSize;
}

void Discrepancy::Synthesizer::WaveUtils::GenerateWaveHeader(void *soundBuf, unsigned int bufferSize, int bps, int numCh, int sampleRate)
{
	WaveHeader *header = (WaveHeader*)soundBuf;
	char *data = (char*)soundBuf + sizeof(WaveHeader); //jumps to beginning of data

	int subChunk1Size = 16; // 16 for PCM
	int subChunk2Size = bufferSize - sizeof(WaveHeader);

	header->chunkID = 0x46464952;       // 0x46464952 "RIFF" in little endian
	header->chunkSize = 4 + (8 + subChunk1Size) + (8 + subChunk2Size);     // 4 + (8 + subChunk1Size) + (8 + subChunk2Size)
	header->format = 0x45564157;        // 0x45564157 "WAVE" in little endian

	header->subChunk1ID = 0x20746d66;   // 0x20746d66 "fmt " in little endian
	header->subChunk1Size = subChunk1Size; // 16 for PCM
	header->audioFormat = 1;   // 1 for PCM, 3 fot EEE floating point , 7 for μ-law
	header->numChannels = numCh;   // 1 for mono, 2 for stereo
	header->sampleRate = sampleRate;    // 8000, 22050, 44100, etc...
	header->byteRate = sampleRate * header->numChannels * bps / 8;      // sampleRate * numChannels * bitsPerSample/8
	header->blockAlign = header->numChannels * bps / 8;    // numChannels * bitsPerSample/8
	header->bitsPerSample = bps; // number of bits (8 for 8 bits, etc...)

	header->subChunk2ID = 0x61746164;   // 0x61746164 "data" in little endian
	header->subChunk2Size = subChunk2Size;// 7000; // numSamples * numChannels * bitsPerSample/8 (this is the actual data size in bytes)
}