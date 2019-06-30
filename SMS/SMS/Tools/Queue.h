#pragma once

#include <Tools/Memory.h>

namespace Discrepancy
{
	namespace Tools
	{
		// FIFO
		template<typename T>
		class Queue
		{
		private:
			unsigned int _size;
			T *_content;

			unsigned int _startSeek;
			unsigned int _endSeek;

		public:
			Queue(unsigned int size) :
				_size(size),
				_content(Memory::HeapAlloc(size * sizeof(T))),
				_startSeek(0U),
				_endSeek(0U)
			{

			}

			Queue() = delete;

			T *Head()
			{
				if (_startSeek >= _endSeek || _startSeek >= _size)
					return nullptr;
				return &(_content[_startSeek]);
			}

			bool Push(T val)
			{
				if (_endSeek < _size)
				{
					_content[_endSeek] = val;
					++_endSeek;
					return true;
				}
				return false;
			}

			bool Pop()
			{
				if (_startSeek < _size && _startSeek < _endSeek)
				{
					++_startSeek;
					return true;
				}
				return false;
			}
		};
	}
}