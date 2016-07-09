/****************************************************************************
Copyright (c) 2016 moon

http://blog.csdn.net/greatchina01

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once
#include "moon_define.hpp"
#include "noncopyable.hpp"

namespace moon
{
	class RingQueue :noncopyable
	{
	public:
		RingQueue();
		~RingQueue();

		void  Write(const uint8_t* pData, uint32_t len);

		bool  Read(uint32_t len,std::vector<uint8_t>& data);

		bool	 Copy(uint8_t* pData, uint32_t len);

		bool  Ship(uint32_t len);

		void reset()
		{
			_in = _out = 0;
		}

		uint32_t getSize()
		{
			return _size;
		}

	private:
		void checkWrite(uint32_t len)
		{
			std::unique_lock<std::mutex> lck(_writemutex);
			while (_capacity - _size < len)
			{
				_notFull.wait(lck);
			}
		}
	private:
		uint8_t *_buffer;				//缓冲区
		uint32_t _capacity;				//缓冲区大小
		std::atomic<uint32_t> _size;    //缓冲区中数据大小
		uint32_t _in;					//当前写的位置
		uint32_t _out;					//当前读的位置

		std::mutex			     _writemutex;
		std::condition_variable  _notFull;
	};

}



