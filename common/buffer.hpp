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
	class buffer;
	using buffer_ptr = std::shared_ptr<buffer>;

	class buffer
	{
		///buffer default size
		constexpr static size_t    INIT_SIZE = 1024;
	public:
		buffer(void)
			:_buffer(INIT_SIZE),
			_read_pos(0),
			_write_pos(0),
			_reservedsize(0)
		{
			assert(size() == 0);
			assert(writeableSize() == INIT_SIZE);
		}

		explicit buffer(size_t _size, size_t headreserved = 0)
			:_buffer(_size + headreserved),
			_read_pos(0),
			_write_pos(0),
			_reservedsize(headreserved)
		{
			_read_pos = _write_pos = _reservedsize;
			assert(size() == 0);
			assert(writeableSize() == _size);
		}

		~buffer(void)
		{

		}

		static buffer_ptr create(size_t size, size_t headreserved = 0)
		{
			return std::make_shared<buffer>(size, headreserved);
		}

		void init(size_t size, size_t headreserved = 0)
		{
			_buffer.resize(size + headreserved);
			_read_pos = _write_pos = headreserved;
			_reservedsize = headreserved;
		}

		void append(const uint8_t* data, size_t len)
		{
			if (0 == len)
				return;

			checkWriteSize(len);
			std::copy(data, data + len, writeable());
			_write_pos += len;
		}

		void push_front(const uint8_t* data, size_t len)
		{
			if (0 == len)
				return;	
			assert(_read_pos >= len);
			_read_pos -= len;
			std::copy(data, data + len, (begin() + _read_pos));
		}

		template<class T>
		void push_front(T value)
		{
			static_assert(std::is_arithmetic<T>::value, "the value must be integer or floating number.");
			push_front((const uint8_t*)&value, sizeof(value));
		}

		uint8_t* data()
		{
			return _buffer.data() + _read_pos;
		}

		//readable size
		uint32_t size() const
		{
			return uint32_t(_write_pos - _read_pos);
		}

		void skip(size_t len)
		{
			if (len < size())
			{
				_read_pos += len;
			}
			else
			{
				_read_pos = _write_pos = 0;
			}
		}

		template<class T>
		buffer& operator<<(T value)
		{
			static_assert(std::is_arithmetic<T>::value, "the value must be integer or floating number.");
			append((uint8_t*)&value, sizeof(value));
			return *this;
		}

		buffer& operator<<(const std::string& value)
		{
			append((uint8_t const*)value.c_str(), value.length());
			(*this)<<((uint8_t)0);
			return *this;
		}

		buffer& operator<<(const char* str)
		{
			append((uint8_t const*)str, str ? strlen(str) : 0);
			(*this) << ((uint8_t)0);
			return *this;
		}


		buffer& operator>>(bool& value)
		{
			uint8_t tmp;
			(*this) >> tmp;
			value = tmp > 0 ? true : false;
			return *this;
		}

		template<class T>
		buffer& operator>>(T& value)
		{
			static_assert(std::is_arithmetic<T>::value, "the value must be integer or floating number.");
			value = read<T>();
			return *this;
		}

		buffer& operator>>(std::string& value)
		{
			value.clear();
			while (_read_pos < _write_pos)
			{
				char c = read<char>();
				if (c == 0)
					break;
				value += c;
			}
			return *this;
		}

		void clear()
		{
			_read_pos = _write_pos = 0;
		}

		std::vector<uint8_t>::iterator writeable()
		{
			return begin() + _write_pos;
		}

	private:
		void checkWriteSize(size_t len)
		{
			if (writeableSize() < len)
			{
				makeSpace(len);
			}
			assert(writeableSize() >= len);
		}

		size_t writeableSize() const
		{
			return _buffer.size() - _write_pos;
		}

		void makeSpace(size_t len)
		{
			if (writeableSize() + _read_pos < len)
			{
				_buffer.resize(_write_pos + len);
			}
			else
			{
				size_t readable = size();
				std::copy(begin() + _read_pos, begin() + _write_pos, begin());
				_read_pos = 0;
				_write_pos = _read_pos + readable;
				assert(readable == size());
			}
		}

		std::vector<uint8_t>::iterator begin()
		{
			return _buffer.begin();
		}

		template <typename T> T read()
		{
			T r = read<T>(_read_pos);
			_read_pos += sizeof(T);
			return r;
		}

		template <typename T> T read(size_t pos) const
		{
			assert(pos + sizeof(T) <= _write_pos);
			T val = *((T const*)&_buffer[pos]);
			return val;
		}

	protected:
		std::vector<uint8_t>		_buffer;
		//read position
		size_t								_read_pos;
		//write position
		size_t								_write_pos;
		//head reserved size
		size_t								_reservedsize;
	};

};



