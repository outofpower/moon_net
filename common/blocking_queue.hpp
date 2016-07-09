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
#include <mutex>
#include <deque>
#include <cassert>
#include "noncopyable.hpp"

namespace moon
{
	template<class T>
	class blocking_queue:noncopyable
	{
	public:
		blocking_queue()
		{
		}

		void push_back(const T& x)
		{
			std::unique_lock<std::mutex> lck(_mutex);
			_queue.push_back(x);
		}

		void emplace_back(T&& v)
		{
			std::unique_lock<std::mutex> lck(_mutex);
			_queue.emplace_back(std::forward<T>(v));
		}

		T pop_front()
		{
			std::unique_lock<std::mutex> lck(_mutex);
			if(_queue.empty())
			{
				return T();
			}
			assert(!_queue.empty());
			T t(_queue.front());
			_queue.pop_front();
			return t;
		}

		size_t size()
		{
			std::unique_lock<std::mutex> lck(_mutex);
			return _queue.size();
		}

		std::deque<T> Move()
		{
			std::unique_lock<std::mutex> lck(_mutex);
			return std::move(_queue);
		}

	private:
		std::mutex			_mutex;
		std::deque<T>		_queue;
	};

}
