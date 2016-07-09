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
#include <functional>
#include <cstdint>
#include <cassert>
#include <memory>
namespace moon
{
	typedef   std::function<void()> timer_handler;

	class timer_context;
	typedef std::shared_ptr<timer_context> timer_context_ptr;

	class timer_context
	{
	public:
		timer_context(const timer_handler& callBack, int64_t duration)
			:m_handler(callBack)
			, m_id(0)
			, m_endtime(0)
			, m_duration(duration)
			, m_repeatTimes(0)
			, m_removed(false)
		{

		}

		timer_context(timer_handler&& callBack, int64_t duration)
			:m_handler(callBack)
			, m_id(0)
			, m_endtime(0)
			, m_duration(duration)
			, m_repeatTimes(0)
			, m_removed(false)
		{
		}

		void init(timer_handler&& callBack, int64_t duration)
		{
			m_handler = std::move(callBack);
			m_endtime = 0;
			m_repeatTimes = 0;
			m_removed = false;
			m_duration = duration;
		}

		void init(const timer_handler& callBack, int64_t duration)
		{
			m_handler = callBack;
			m_endtime = 0;
			m_repeatTimes = 0;
			m_removed = false;
			m_duration = duration;
		}

		void setID(uint32_t value)
		{
			m_id = value;
		}

		uint32_t getID()
		{
			return m_id;
		}

		void setEndtime(int64_t value)
		{
			m_endtime = value;
		}

		int64_t getEndtime()
		{
			return m_endtime;
		}

		int64_t getDuration()
		{
			return m_duration;
		}

		void setRepeatTimes(int32_t value)
		{
			m_repeatTimes = value;
		}

		int32_t getRepeatTimes()
		{
			return m_repeatTimes;
		}

		void  expired()
		{
			assert(nullptr != m_handler);
			m_handler();
		}

		void setRemoved(bool value)
		{
			m_removed = value;
		}

		bool getRemoved()
		{
			return m_removed;
		}
	private:
		timer_handler				m_handler;//调度器回掉
		uint32_t						m_id;
		int64_t							m_endtime;
		int64_t							m_duration;
		int32_t							m_repeatTimes;//重复次数，-1循环
		bool								m_removed;
	};
}

