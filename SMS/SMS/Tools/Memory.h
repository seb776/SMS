#pragma once

namespace Discrepancy
{
	namespace Memory
	{
		void Memset(char *data, int size);
		void MemCpy(char *dst, const char *src, int size);
		void *HeapAlloc(unsigned int size);
		template<typename T>
		T *HeapAlloc()
		{
			return (T*)HeapAlloc(sizeof(T));
		}
		void HeapFree(void *ptr);

		extern unsigned int AllocationCount;
		extern unsigned int DeallocationCount;
	}
}

#define NEW(type) new(Memory::HeapAlloc<type>()) type

//void* operator new(size_t sz);
//void operator delete(void* ptr) noexcept;


//extern "C" void * __cdecl memcpy(void *dst, const void *src, size_t size);

#pragma intrinsic(memcpy)