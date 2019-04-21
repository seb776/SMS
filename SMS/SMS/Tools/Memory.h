#pragma once

namespace Discrepancy
{
	namespace Memory
	{
		void Memset(char *data, int size);
		void MemCpy(char *dst, const char *src, int size);
		void *HeapAlloc(unsigned int size);
		void HeapFree(void *ptr);
	}
}

//extern "C" void * __cdecl memcpy(void *dst, const void *src, size_t size);

#pragma intrinsic(memcpy)