#include "MeanLeanWindows.h"
#include "Memory.h"

using namespace Discrepancy;

#pragma optimize( "gst", off )
void Memory::Memset(char *data, int size)
{
	for (int i = 0; i < size; ++i)
		data[i] = 0;
}
#pragma optimize( "gst", off )
void Memory::MemCpy(char *dst, const char *src, int size)
{
	for (int i = 0; i < size; ++i)
		dst[i] = src[i];
}

void *Memory::HeapAlloc(unsigned int size)
{
	return (void*)::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (SIZE_T)size);
	Discrepancy::Memory::AllocationCount++;
}

void Memory::HeapFree(void *ptr)
{
	::HeapFree(GetProcessHeap(), 0, (LPVOID)ptr);
	Discrepancy::Memory::DeallocationCount++;
}

unsigned int Memory::AllocationCount = 0U;
unsigned int Memory::DeallocationCount = 0U;