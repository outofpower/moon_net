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
#include "memory_stream.hpp"

namespace moon
{
	class binary_writer
	{
	public:
		explicit binary_writer(memory_stream_ptr& ms)
			:m_ms(*ms)
		{
		}

		explicit binary_writer(memory_stream& ms)
			:m_ms(ms)
		{
		}

		~binary_writer(void)
		{

		}

		binary_writer(const binary_writer& t) = delete;
		binary_writer& operator=(const binary_writer& t) = delete;

		template<typename T>
		void write_front(T& t)
		{
			m_ms.write_front(&t, 0, 1);
		}

		template<typename T>
		void write_front(T* t,size_t count)
		{
			m_ms.write_front(t, 0, count);
		}

		template<typename T>
		void write(const T& t)
		{
			static_assert(std::is_pod<T>::value, "type T must be pod.");
			m_ms.write_back(&t, 0, 1);
		}

		void write(const char* str)
		{
			m_ms.write_back(str, 0, str ? strlen(str) + 1 : 0);
		}

		void write(const std::string& str)
		{
			m_ms.write_back(str.data(), 0, str.size() + 1);
		}


		template<typename T>
		void write_vector(const std::vector<T>& t)
		{
			static_assert(std::is_pod<T>::value, "type T must be pod.");
			write(t.size());
			for (auto& it : t)
			{
				write(it);
			}
		}

		template<typename T>
		void write(T* t, size_t count)
		{
			static_assert(std::is_pod<T>::value, "type T must be pod.");
			m_ms.write_back(&t, 0, count);
		}

		template<class T>
		binary_writer& operator<<(T& value)
		{
			write(value);
			return *this;
		}

	protected:
		memory_stream&			m_ms;
	};

};




