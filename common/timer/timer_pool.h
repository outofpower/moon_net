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
#include <unordered_map>
#include <memory>
#include "timer_wheel.hpp"
#include "timer_context.hpp"

namespace moon
{
	class timer_pool
	{	
	public:
		//每个时间轮子大小，最大255
		static const  uint8_t  wheel_size = 255;
		//精度 ms
		static const int32_t PRECISION = 15;

		timer_pool();

		~timer_pool();
		/**
		* 添加一个只执行一次的计时器
		*
		* @duration 计时器间隔 ms
		* @callBack  回掉函数
		*  typedef   std::function<void()> timer_handler;
		* 返回计时器ID
		*/
		uint32_t  expiredOnce(int64_t duration,const timer_handler& callBack);

		uint32_t  expiredOnce(int64_t duration,timer_handler&& callBack);

		/**
		* 添加重复执行的计时器
		*
		* @duration 计时器间隔 ms
		* @times 重复次数，(-1 无限循环)
		* @callBack 回掉函数
		*  typedef   std::function<void()> timer_handler;
		* 返回计时器ID
		*/
		uint32_t  repeat(int64_t duration, int32_t times, timer_handler&& callBack);

		uint32_t  repeat(int64_t duration, int32_t times, const timer_handler& callBack);

		/**
		* 移除一个计时器
		*
		* @timerid 计时器 ID
		*/
		void remove(uint32_t timerid);
		
		//主逻辑需要调用这个函数，驱动计时器
		void update();

		void stopAllTimer();

		void startAllTimer();

		static int64_t millseconds();
	private:
		uint32_t 	addNewTimer(const timer_context_ptr& t);
		void			addTimer(const timer_context_ptr& t);
		void			expired(const std::vector<uint64_t>& timers);
		uint64_t	makeKey(uint32_t id, uint32_t  slots);
		uint8_t		getSlot(uint64_t  key, int which_queue);	
	private:
		std::vector < timer_wheel<std::vector<uint64_t>, wheel_size>> m_wheels;
		std::unordered_map<uint32_t, timer_context_ptr>		m_timers;
		std::vector<timer_context_ptr>										m_new;
		int64_t																				m_tick;
		uint32_t																			m_inc;
		bool																					m_Stop;
	};
}

