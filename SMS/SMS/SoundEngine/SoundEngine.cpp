#include "../Tools/MeanLeanWindows.h"
#include <MMSystem.h>
#include "../Tools/Memory.h"
#include "../Tools/Math.h"
#include "SoundEngine.h"

using namespace Discrepancy;

#define SOUND_BUFFER_SIZE (8000 * 8)

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

// Help
// Sound https://stackoverflow.com/questions/1451606/programably-make-and-play-a-sound-through-speakers-c
// http://soundfile.sapp.org/doc/WaveFormat/

Discrepancy::SoundEngine::SoundEngine(float duration)
{
	_buffer = Memory::HeapAlloc(SOUND_BUFFER_SIZE);
}

Discrepancy::SoundEngine::~SoundEngine()
{
	Memory::HeapFree(_buffer);
}

// bps = bit per sample
// numCh = numChannels
// 
void GenerateWaveHeader(LPCSTR soundBuf, unsigned int bufferSize, int bps, int numCh, int sampleRate)
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
	header->byteRate = sampleRate * header->numChannels * 8 / 8;      // sampleRate * numChannels * bitsPerSample/8
	header->blockAlign = header->numChannels * bps/8;    // numChannels * bitsPerSample/8
	header->bitsPerSample = bps; // number of bits (8 for 8 bits, etc...)

	header->subChunk2ID = 0x61746164;   // 0x61746164 "data" in little endian
	header->subChunk2Size = subChunk2Size;// 7000; // numSamples * numChannels * bitsPerSample/8 (this is the actual data size in bytes)


}

void Discrepancy::SoundEngine::Generate()
{
	LPCSTR soundBuf = (LPCSTR)_buffer;

	GenerateWaveHeader(soundBuf, SOUND_BUFFER_SIZE, 8, 1, 8000);

	// duration d
	float d = 60.0;
	int bps = 8;
	int sampleRate = 8000;
	int channels = 2;
	int bufferSize = channels * bps*sampleRate*(int)Math::Ceil(d);

	WaveHeader *header = (WaveHeader*)soundBuf;
	// fill out the header...

	char *data = (char*)soundBuf + sizeof(WaveHeader); //jumps to beginning of data

	header->chunkID = 0x46464952;       // 0x46464952 "RIFF" in little endian
	header->chunkSize = 4 + (8 + 16) + (8 + SOUND_BUFFER_SIZE - sizeof(WaveHeader));     // 4 + (8 + subChunk1Size) + (8 + subChunk2Size)
	header->format = 0x45564157;        // 0x45564157 "WAVE" in little endian

	header->subChunk1ID = 0x20746d66;   // 0x20746d66 "fmt " in little endian
	header->subChunk1Size = 16; // 16 for PCM
	header->audioFormat = 1;   // 1 for PCM, 3 fot EEE floating point , 7 for μ-law
	header->numChannels = 1;   // 1 for mono, 2 for stereo
	header->sampleRate = 8000;    // 8000, 22050, 44100, etc...
	header->byteRate = header->sampleRate * header->numChannels * 8 / 8;      // sampleRate * numChannels * bitsPerSample/8
	header->blockAlign = 1;    // numChannels * bitsPerSample/8
	header->bitsPerSample = 8; // number of bits (8 for 8 bits, etc...)

	header->subChunk2ID = 0x61746164;   // 0x61746164 "data" in little endian
	header->subChunk2Size = SOUND_BUFFER_SIZE - sizeof(WaveHeader);// 7000; // numSamples * numChannels * bitsPerSample/8 (this is the actual data size in bytes)


	for (int i = sizeof(WaveHeader); i < SOUND_BUFFER_SIZE; ++i)
	{
		((unsigned char*)(soundBuf))[i] = (unsigned char)(Math::Sin(i*0.5) * 127.0f) + 126U;
	}
}

void Discrepancy::SoundEngine::Play()
{
	LPCSTR soundBuf = (LPCSTR)_buffer;

	PlaySound(soundBuf, NULL, SND_MEMORY | SND_ASYNC);
}
