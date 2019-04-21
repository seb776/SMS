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

Discrepancy::SoundEngine::SoundEngine() :
	_buffer(nullptr)
{

}

Discrepancy::SoundEngine::~SoundEngine()
{
	if (_buffer != nullptr)
		Memory::HeapFree(_buffer);
}

// bps = bit per sample
// numCh = numChannels
// 
void GenerateWaveHeader(void *soundBuf, unsigned int bufferSize, int bps, int numCh, int sampleRate)
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

// does not take WaveHeader into account
int CalculateBufferSize(int bps, int numCh, int sampleRate, float duration)
{
	int bufferSize = numCh * bps*sampleRate*(int)Math::Ceil(duration);
	return bufferSize;
}

char *putnbr(int nbr, char *buf, int size)
{
	int i;
	for (i = 0; nbr > 0 && i < size; nbr /= 10, ++i)
	{
		buf[(size - 1) - i] = '0' + nbr % 10;
	}
	return &(buf[(size)-i]);
}

void Discrepancy::SoundEngine::Generate(float duration)
{
	int bps = 8;
	int sampleRate = 8000;
	int channels = 2;

	int bufferSize = CalculateBufferSize(bps, channels, sampleRate, duration);

	_buffer = Memory::HeapAlloc(bufferSize + sizeof(WaveHeader));
	Memory::Memset((char*)_buffer, bufferSize);

	GenerateWaveHeader(_buffer, bufferSize, bps, channels, sampleRate);


	WaveHeader *header = (WaveHeader*)_buffer;
	unsigned char *data = (unsigned char *)_buffer + sizeof(WaveHeader);

	char nbr[16];
	for (int i = sizeof(WaveHeader); i < bufferSize; i += 2)
	{
		data[i] = i % 200;// (unsigned char)(Math::Sin(i*0.5) * 127.0f) + 126U;
		if ((i % 100) == 0)
		{
			Memory::Memset(nbr, 16);
			OutputDebugString(putnbr(i, nbr, 15));
			OutputDebugStringW(L"/");
			Memory::Memset(nbr, 16);
			OutputDebugString(putnbr(bufferSize, nbr, 15));
			OutputDebugStringW(L"\n");
		}
	}
}

void Discrepancy::SoundEngine::Play()
{
	LPCSTR soundBuf = (LPCSTR)_buffer;

	PlaySound(soundBuf, NULL, SND_MEMORY | SND_ASYNC);
}
