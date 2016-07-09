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
#include <cstdint>
#include <cassert>

namespace moon
{
	template<typename _Type, uint8_t _Size>
	class timer_wheel
	{
	public:
		timer_wheel()
			:m_head(1)
		{
		}

		_Type& operator[](uint8_t pos)
		{
			assert(pos < _Size);
			return m_array[pos];
		}

		_Type& front()
		{
			assert(m_head < _Size);
			return m_array[m_head];
		}

		void pop_front()
		{
			m_head = (++m_head) % _Size;
		}

		bool round()
		{
			return m_head == 0;
		}

		uint8_t size()
		{
			return _Size;
		}

		size_t next_slot() { return m_head; }
	private:
		_Type		m_array[_Size];
		uint32_t	m_head;
	};
};

