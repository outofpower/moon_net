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
#include <fstream>
#include <vector>
#include <string>

namespace
{
	class File
	{
	public:
		static std::string ReadAllText(const std::string& path, int Mode = std::fstream::in)
		{
			std::fstream is(path, Mode);
			if (is.is_open())
			{
				// get length of file:
				is.seekg(0, is.end);
				size_t length = is.tellg();
				is.seekg(0, is.beg);

				std::string tmp(length, '\0');
				is.read(&tmp.front(), length);

				if (is)
				{
					//all characters read successfully
				}
				else
				{
					//error: only " << is.gcount() << " could be read
				}
				is.close();
				return std::move(tmp);
			}
			return std::string();
		}


		static std::vector<char> ReadAllBytes(const std::string& path, int Mode = std::ios::binary | std::ios::in)
		{
			std::fstream is(path, Mode);
			if (is.is_open())
			{
				// get length of file:
				is.seekg(0, is.end);
				size_t length = is.tellg();
				is.seekg(0, is.beg);

				std::vector<char> tmp(length);
				is.read(&tmp[0], length);
				if (is)
				{
					//all characters read successfully
				}
				else
				{
					//error: only " << is.gcount() << " could be read
				}
				is.close();
				return std::move(tmp);
			}
			return std::vector<char>();
		}


		static size_t GetFileSize(const std::string& path)
		{
			std::ifstream is(path, std::ifstream::binary);
			if (is) {
				// get length of file:
				is.seekg(0, is.end);
				auto length = is.tellg();
				is.close();
				return length;

			}
			return 0;
		}
	};
}

