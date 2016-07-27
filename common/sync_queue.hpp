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
#include <condition_variable>
#include <deque>
#include <cassert>

namespace moon
{
	template<class T,size_t max_size = 50>
	class sync_queue
	{
	public:
		sync_queue()
			:m_exit(false)
		{
		}

		sync_queue(const sync_queue& t) = delete;
		sync_queue& operator=(const sync_queue& t) = delete;

		void push_back(const T& x)
		{
			std::unique_lock<std::mutex> lck(_mutex);

			m_notFull.wait(lck, [this] {return m_exit || (_queue.size() < max_size); });

			_queue.push_back(x);
		}

		template<typename _Tdata>
		void emplace_back(_Tdata&& v)
		{
			std::unique_lock<std::mutex> lck(_mutex);
			_queue.emplace_back(std::forward<_Tdata>(v));
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

		std::deque<T> move()
		{
			std::unique_lock<std::mutex> lck(_mutex);
			auto tmp = std::move(_queue);
			m_notFull.notify_one();
			return std::move(tmp);
		}

	private:
		std::mutex			_mutex;
		std::condition_variable m_notFull;
		std::deque<T>		_queue;
		bool						m_exit;
	};

}
