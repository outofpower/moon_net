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

		buffer(buffer&& other)
			:_buffer(std::move(other._buffer)),
			_read_pos(other._read_pos),
			_reservedsize(other._reservedsize),
			_write_pos(other._write_pos)
		{

		}

		buffer& operator=(const buffer&& other) = delete;

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

		template<class T>
		T convert()
		{
			if (std::is_pointer::value)
			{
				return (T*)data();
			}
			else
			{
				return *(T*)data();
			}
		}

		//template<class T>
		//T pop_front()
		//{
		//	T tmp;
		//	(*this) >> tmp;
		//	return tmp;
		//}

		//template<class T>
		//void push_back(const T& v)
		//{
		//	(*this) << v;
		//}

		template<class T>
		buffer& operator<<(T value)
		{
			static_assert(std::is_arithmetic<T>::value, "the value must be pod type.");
			append((uint8_t*)&value, sizeof(value));
			return *this;
		}

		buffer& operator<<(const std::string& value)
		{
			append((uint8_t const*)value.c_str(), value.length());
			(*this) << ((uint8_t)0);
			return *this;
		}

		buffer& operator<<(const char* str)
		{
			append((uint8_t const*)str, str ? strlen(str) : 0);
			(*this) << ((uint8_t)0);
			return *this;
		}

		template<class T>
		buffer& operator<<(const std::vector<T>& vec)
		{
			(*this) << vec.size();
			for (auto& it : vec)
			{
				(*this) << it;
			}
			return *this;
		}


		buffer& operator >> (bool& value)
		{
			uint8_t tmp;
			(*this) >> tmp;
			value = tmp > 0 ? true : false;
			return *this;
		}

		template<class T>
		buffer& operator >> (T& value)
		{
			static_assert(std::is_arithmetic<T>::value, "the value must be pod type.");
			value = read<T>();
			return *this;
		}

		buffer& operator >> (std::string& value)
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

		template<class T>
		buffer& operator >> (std::vector<T>& vec)
		{
			vec.clear();
			size_t vec_size = 0;
			(*this) >> vec_size;
			for (size_t i = 0; i < vec_size; i++)
			{
				T tmp;
				(*this) >> tmp;
				vec.push_back(tmp);
			}

			return *this;
		}

		uint8_t* data()
		{
			return _buffer.data() + _read_pos;
		}

		const uint8_t* data() const
		{
			return _buffer.data() + _read_pos;
		}

		//readable size
		size_t size() const
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

		void skip(size_t len) const
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

		template <typename T> 
		T read()
		{
			T r = read<T>(_read_pos);
			_read_pos += sizeof(T);
			return r;
		}

		template <typename T> 
		T read(size_t pos) const
		{
			assert(pos + sizeof(T) <= _write_pos);
			T val = *((T const*)&_buffer[pos]);
			return val;
		}
	protected:
		std::vector<uint8_t>		_buffer;
		//read position
		mutable size_t					_read_pos;
		//write position
		mutable size_t					_write_pos;
		//head reserved size
		size_t								_reservedsize;
	};

};



