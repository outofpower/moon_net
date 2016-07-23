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
#include <algorithm>
#include <cstdint>
#include <vector>
#include <cassert>
#include <memory>
#include <type_traits>

namespace moon
{
	class		memory_stream;
	using	memory_stream_ptr = std::shared_ptr<memory_stream>;

	class memory_stream
	{
		///buffer default size
		constexpr static size_t    DEFAULT_CAPACITY = 64;
	public:
		enum seek_origin
		{
			Begin,
			Current,
			End
		};

		memory_stream(void)
			:m_data(DEFAULT_CAPACITY),
			m_readpos(0),
			m_writepos(0),
			m_headreserved(0)
		{
			assert(size() == 0);
			assert(write_able_size() == DEFAULT_CAPACITY);
		}

		memory_stream(size_t capacity)
			:m_data(capacity),
			m_readpos(0),
			m_writepos(0),
			m_headreserved(0)
		{
			assert(size() == 0);
			assert(write_able_size() == capacity);
		}

		memory_stream(size_t capacity,size_t headreserved)
			:m_data(capacity+ headreserved),
			m_readpos(headreserved),
			m_writepos(headreserved),
			m_headreserved(headreserved)
		{
			assert(capacity >= headreserved && "capacity must be equal or greater than headreserved");
			assert(size() == 0);
			assert(write_able_size() == capacity);
		}

		~memory_stream(void)
		{
		}

		memory_stream(const memory_stream& t) = delete;
		memory_stream& operator=(const memory_stream& t) = delete;

		template<typename... Args>
		static memory_stream create(Args&&... args)
		{
			return std::make_shared<memory_stream>(std::forward<Args>(args)...);
		}

		void init()
		{
			m_data.resize(DEFAULT_CAPACITY+m_headreserved);
			m_readpos = m_writepos = m_headreserved;
		}

		template<typename T,typename _T = std::remove_cv<T>::type>
		void write_back(T* Indata, size_t offset, size_t count)
		{
			static_assert(!std::is_pointer<_T>::value, "type T must be not pointer");
			if (nullptr == Indata || 0 == count)
				return;
			check_write_able_size(sizeof(_T)*count);
			auto* buff = (_T*)&(*writeable());

			for (size_t i = 0; i < count; i++)
			{
				buff[i] = Indata[offset + i];
			}

			m_writepos += sizeof(_T)*count;
		}

		template<typename T, typename _T = std::remove_cv<T>::type>
		void write_front(T* Indata, size_t offset, size_t count)
		{
			static_assert(!std::is_pointer<_T>::value, "type T must be not pointer");
			if (nullptr == Indata || 0 == count)
				return;
			
			if (sizeof(_T)*count > m_readpos)
			{
				count = m_readpos / sizeof(_T);
			}
	
			m_readpos -= sizeof(_T)*count;

			auto* buff = (_T*)&m_data[m_readpos];
			for (size_t i = 0; i < count; i++)
			{
				buff[i] = Indata[offset + i];
			}
		}

		template<typename T>
		void read(T* Outdata, size_t offset, size_t count)
		{
			static_assert(!std::is_pointer<T>::value, "type T must be not pointer.");
			static_assert(!std::is_const<T>::value, "Outdata must be not const.");
			if (nullptr == Outdata || 0 == count)
				return;

			if (m_readpos + sizeof(T)*count > m_writepos)
			{
				count = (m_writepos - m_readpos) / sizeof(T);
			}

			auto* buff = (T*)&m_data[m_readpos];

			for (size_t i = 0; i < count; i++)
			{
				Outdata[i + offset] = buff[i];
			}
			m_readpos += sizeof(T)*count;
		}

		const uint8_t* data() const
		{
			return m_data.data() + m_readpos;
		}

		//readable size
		uint32_t size() const
		{
			return uint32_t(m_writepos - m_readpos);
		}

		size_t seek(int offset, seek_origin s)
		{
			switch (s)
			{
			case Begin:
				m_readpos = offset;
				break;
			case Current:
				m_readpos += offset;
				if (m_readpos >= m_writepos)
				{
					m_readpos = m_writepos;
				}
				break;
			case End:
				m_readpos = m_writepos + offset;
				if (m_readpos >= m_writepos)
				{
					m_readpos = m_writepos;
				}
				break;
			default:
				break;
			}

			return m_readpos;
		}

		void clear()
		{
			m_readpos = m_writepos = m_headreserved = 0;
		}

		std::vector<uint8_t>::iterator writeable()
		{
			return begin() + m_writepos;
		}

	private:
		size_t write_able_size()
		{
			return m_data.size() - m_writepos;
		}

		void check_write_able_size(size_t len)
		{
			if (write_able_size() < len)
			{
				make_space(len);
			}
			assert(write_able_size() >= len);
		}

		void make_space(size_t len)
		{
			if (write_able_size() + m_readpos +m_headreserved < len)
			{
				size_t s = DEFAULT_CAPACITY;
				while (s < m_writepos + len)
				{
					s *= 2;
				}
				m_data.resize(s);
			}
			else
			{
				size_t readable = size();
				std::copy(begin() + m_readpos, begin() + m_writepos, begin());
				m_readpos = 0;
				m_writepos = m_readpos + readable;
			}
		}

		std::vector<uint8_t>::iterator begin()
		{
			return m_data.begin();
		}
	protected:
		std::vector<uint8_t>		m_data;
		//read position
		size_t								m_readpos;
		//write position
		size_t								m_writepos;
		//head reserved size
		size_t								m_headreserved;
	};

};



