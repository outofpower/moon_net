#pragma once
#include <cstdint>
#include <cassert>
#include <memory>
#include <type_traits>
#include <string>

namespace moon
{
	class binary_reader
	{
	public:
		binary_reader(const uint8_t* data, size_t size)
			:m_data(data), m_size(size), m_readpos(0)
		{
		}

		binary_reader(const binary_reader&) = delete;
		binary_reader& operator=(const binary_reader&) = delete;

		~binary_reader(void)
		{
		}

		template<typename T>
		T read(T* t = nullptr)
		{
			static_assert(std::is_pod<T>::value, "type T must be pod.");
			return _read<T>();
		}

		template<typename T>
		T read(typename std::enable_if<std::is_same<T, bool>::value>::type* t = nullptr)
		{
			return (_read<uint8_t>() != 0) ? true : false;
		}

		template<typename T>
		T read(typename std::enable_if<std::is_same<T,std::string>::value>::type* t = nullptr)
		{
			std::string tmp;
			while (m_readpos < m_size)
			{
				char c = _read<char>();
				if (c == 0)
					break;
				tmp += c;
			}
			return std::move(tmp);
		}


		template<class T>
		std::vector<T> read_vector()
		{
			std::vector<T> vec;
			size_t vec_size = 0;
			(*this) >> vec_size;
			for (size_t i = 0; i < vec_size; i++)
			{
				T tmp;
				(*this) >> tmp;
				vec.push_back(tmp);
			}
			return std::move(vec);
		}


		template<class T>
		binary_reader& operator >> (T& value)
		{
			value = read<T>();
			return *this;
		}

		const uint8_t* data() const
		{
			return m_data + m_readpos;
		}

		//readable size
		uint32_t size() const
		{
			return uint32_t(m_size - m_readpos);
		}

		void skip(size_t len)
		{
			if (len < size())
			{
				m_readpos += len;
			}
			else
			{
				m_readpos = m_size = 0;
			}
		}

	private:
		template <typename T>
		T _read()
		{
			T r = _read<T>(m_readpos);
			m_readpos += sizeof(T);
			return r;
		}

		template <typename T>
		T _read(size_t pos) const
		{
			assert((pos + sizeof(T) <= m_size) && "memory out of size.");
			T val = *((T const*)&m_data[pos]);
			return val;
		}
	protected:
		const uint8_t*					m_data;
		//_read position
		size_t								m_readpos;
		//write position
		size_t								m_size;
	};

};



