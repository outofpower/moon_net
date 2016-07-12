#pragma once
#include <cstdint>
#include <cassert>
#include <memory>
#include <type_traits>
namespace moon
{
	class buffer_reader
	{
	public:
		buffer_reader(const uint8_t* data, size_t size)
			:_data(data),_size(size),_read_pos(0)
		{
		}

		buffer_reader(const buffer_reader&) = delete;

		buffer_reader& operator=(const buffer_reader&) = delete;

		~buffer_reader(void)
		{

		}

		buffer_reader& operator >> (bool& value)
		{
			uint8_t tmp;
			(*this) >> tmp;
			value = tmp > 0 ? true : false;
			return *this;
		}

		template<class T>
		buffer_reader& operator >> (T& value)
		{
			static_assert(std::is_arithmetic<T>::value, "the value must be integer or floating number.");
			value = read<T>();
			return *this;
		}

		buffer_reader& operator >> (std::string& value)
		{
			value.clear();
			while (_read_pos < _size)
			{
				char c = read<char>();
				if (c == 0)
					break;
				value += c;
			}
			return *this;
		}

		template<class T>
		buffer_reader& operator >> (std::vector<T>& vec)
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

		const uint8_t* data() const
		{
			return _data+ _read_pos;
		}

		//readable size
		uint32_t size() const
		{
			return uint32_t(_size - _read_pos);
		}

		void skip(size_t len)
		{
			if (len < size())
			{
				_read_pos += len;
			}
			else
			{
				_read_pos = _size = 0;
			}
		}

	private:
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
			assert(pos + sizeof(T) <= _size);
			T val = *((T const*)&_data[pos]);
			return val;
		}
	protected:
		const uint8_t*					_data;
		//read position
		size_t								_read_pos;
		//write position
		size_t								_size;
	};

};



