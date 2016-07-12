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
#include <cstdint>
#include <chrono>
#include <thread>

namespace moon
{
	class loop_thread
	{
	public:
		loop_thread()
			:_Interval(1000), _bStop(false)
		{

		}

		~loop_thread()
		{

		}

		loop_thread(const loop_thread& t) = delete;
		loop_thread& operator=(const loop_thread& t) = delete;

		//…Ë÷√—≠ª∑º‰∏Ù ∫¡√Î
		void set_interval(uint32_t interval)
		{
			_Interval = interval;
		}

		virtual void run()
		{
			_Thread = std::thread(std::bind(&loop_thread::loop, this));
		}

		virtual void stop()
		{
			_bStop = true;
			_Thread.join();
		}

		std::function<void(uint32_t)> onUpdate;
	private:
		void loop()
		{
			uint64_t prew = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			uint32_t prevSleepTime = 0;
			while (!_bStop)
			{
				uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				uint32_t diff = (now - prew) & 0xFFFFFFFF;
				prew = now;

				if (onUpdate != nullptr)
				{
					onUpdate(diff);
				}

				if (diff <= _Interval + prevSleepTime)
				{
					prevSleepTime = _Interval + prevSleepTime - diff;
					std::this_thread::sleep_for(std::chrono::milliseconds(prevSleepTime));
				}
				else
					prevSleepTime = 0;
			}
		}

	private:
		uint32_t			_Interval;
		std::thread		_Thread;
		bool					_bStop;
	};
}
