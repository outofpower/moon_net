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
#include <stdarg.h>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>

#define MAX_FMT_LEN 32*1024

//×Ö·û´®´¦Àí°ïÖúº¯Êý
namespace string_utils
{
	template<class T>
	T string_convert(const std::string& s);

	template<>
	inline std::string string_convert<std::string>(const std::string& s)
	{
		return s;
	}

	template<>
	inline int32_t string_convert<int32_t>(const std::string& s)
	{
		return std::stoi(s);
	}

	template<>
	inline uint32_t string_convert<uint32_t>(const std::string& s)
	{
		return std::stoul(s)&(0xFFFFFFFF);
	}

	template<>
	inline int64_t string_convert<int64_t>(const std::string& s)
	{

		return std::stoll(s);
	}

	template<>
	inline uint64_t string_convert<uint64_t>(const std::string& s)
	{
		return std::stoull(s);
	}

	template<>
	inline float string_convert<float>(const std::string& s)
	{
		return std::stof(s);
	}

	template<>
	inline double string_convert<double>(const std::string& s)
	{
		return std::stod(s);
	}

	inline std::string to_string(const std::string& s)
	{
		return s;
	}

	inline std::string to_string(int32_t v)
	{
		return std::to_string(int64_t(v));
	}

	inline std::string to_string(int64_t v)
	{
		return std::to_string(v);
	}

	inline std::string to_string(uint32_t v)
	{
		return std::to_string(uint64_t(v));
	}

	inline std::string to_string(uint64_t v)
	{
		return std::to_string(v);
	}

	inline std::string to_string(float v)
	{
		return std::to_string(long double(v));
	}

	inline std::string to_string(double v)
	{
		return std::to_string(long double(v));
	}

	/*
	·Ö¸î×Ö·û´®
	e. split("aa/bb/cc","/")
	*/
	template<class T>
	std::vector<T> split(const std::string &src, const std::string &sep)
	{
		std::vector<T> r;
		std::string s;
		for (std::string::const_iterator i = src.begin(); i != src.end(); i++)
		{
			if (sep.find(*i) != std::string::npos)
			{
				if (s.length()) r.push_back(string_convert<T>(s));
				s = "";
			}
			else
			{
				s += *i;
			}
		}
		if (s.length()) r.push_back(string_convert<T>(s));
		return std::move(r);
	}

	//¸ñÊ½»¯×Ö·û´®
	inline std::string				format(const char* fmt, ...)
	{
		if (!fmt) return std::string("");
		va_list ap;
		char szBuffer[MAX_FMT_LEN];
		va_start(ap, fmt);
		int ret = 0;
		// win32
#if defined(_WIN32)
		ret = vsnprintf_s(szBuffer, MAX_FMT_LEN, fmt, ap);
#endif
		// linux
#if defined(LINUX)
		ret = vsnprintf(szBuffer, MAX_FMT_LEN, fmt, ap);
#endif
		va_end(ap);
		return std::string(szBuffer);
	}

	//·µ»Ø×ó±ßn¸ö×Ö·û
	inline std::string				left(const std::string &str, size_t n)
	{
		return std::string(str, 0, n);
	}

	//·µ»ØÓÒ±ßn¸ö×Ö·û
	inline std::string				right(const std::string &str, size_t n)
	{
		size_t s = (str.size()>=n)?(str.size() - n):0;
		return std::string(str, s);
	}

	//Ïû³ýÓÒ±ß" /t/n/r"
	inline void						trimright(std::string &str)
	{
		str.erase(str.find_last_not_of(" /t/n/r" + 1));
	}

	inline void						trimleft(std::string &str)
	{
		str.erase(str.find_first_not_of(" /t/n/r" + 1));
	}

	//×Ö·û´®Ìæ»»
	inline void						replace(std::string& src, const std::string& old, const std::string& strnew)
	{
		for (std::string::size_type pos(0); pos != std::string::npos; pos += strnew.size()) {
			if ((pos = src.find(old, pos)) != std::string::npos)
				src.replace(pos, old.size(), strnew);
			else
				break;
		}
	}

	inline void						upper(std::string& src)
	{
		std::transform(src.begin(), src.end(), src.begin(), ::toupper);
	}

	inline void						lower(std::string& src)
	{
		std::transform(src.begin(), src.end(), src.begin(), ::tolower);
	}
};
