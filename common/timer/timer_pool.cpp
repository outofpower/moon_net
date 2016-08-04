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

#include "timer_pool.h"
#include <cassert>
#include <time.h>

// win32
#if defined(_WIN32)// && defined(_WINDOWS)
#include <Windows.h>
struct stimezone
{
};

static int64_t gettimeofday(timeval* val, stimezone*)
{
	if (val)
	{
		LARGE_INTEGER liTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liTime);
		val->tv_sec = (long)(liTime.QuadPart / liFreq.QuadPart);
		val->tv_usec = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0);
	}
	return 0;
}
#endif

static int64_t getus(timeval t)
{
	return(t.tv_sec * 1000000 + t.tv_usec);
}

static long getms(timeval t)
{
	return(t.tv_sec * 1000 + t.tv_usec / 1000);
}

using namespace moon;

timer_pool::timer_pool()
:m_tick(0),m_inc(0), m_Stop(false)
{
	m_wheels.emplace_back(timer_wheel<std::vector<uint64_t>, wheel_size>());
	m_wheels.emplace_back(timer_wheel<std::vector<uint64_t>, wheel_size>());
	m_wheels.emplace_back(timer_wheel<std::vector<uint64_t>, wheel_size>());
	m_wheels.emplace_back(timer_wheel<std::vector<uint64_t>, wheel_size>());
}

timer_pool::~timer_pool()
{
}

uint32_t  moon::timer_pool::expiredOnce(int64_t duration, const timer_handler& callBack)
{
	assert(duration > 0);
	auto pt = std::make_shared<timer_context>(callBack, duration);
	return addNewTimer(pt);
}

uint32_t  moon::timer_pool::repeat(int64_t duration, int32_t times, timer_handler&& callBack)
{
	assert(duration > 0);
	auto pt = std::make_shared<timer_context>(std::forward<timer_handler>(callBack), duration);
	pt->setRepeatTimes(times);
	return addNewTimer(pt);
}

void moon::timer_pool::remove(uint32_t timerid)
{
	auto iter = m_timers.find(timerid);
	if (iter != m_timers.end())
	{
		iter->second->setRemoved(true);
		return;
	}

	for (auto& nit = m_new.begin(); nit != m_new.end(); nit++)
	{
		if ((*nit)->getID() == timerid)
		{
			(*nit)->setRemoved(true);
			break;
		}
	}
}

uint32_t  moon::timer_pool::repeat(int64_t duration, int32_t times, const timer_handler& callBack)
{
	assert(duration > 0);
	auto pt = std::make_shared<timer_context>(callBack, duration);
	pt->setRepeatTimes(times);
	return addNewTimer(pt);
}

uint32_t  moon::timer_pool::expiredOnce(int64_t duration, timer_handler&& callBack)
{
	assert(duration > 0);
	auto pt = std::make_shared<timer_context>(std::forward<timer_handler>(callBack), duration);
	return addNewTimer(pt);
}

void timer_pool::update()
{
	for (auto& itnew = m_new.begin(); itnew != m_new.end() ; itnew++)
	{
		if((*itnew)->getRemoved())
			continue;
		addTimer(*itnew);
	}
	m_new.clear();

	static auto prewTick = millseconds();
	auto nowTick = millseconds();
	m_tick += (nowTick - prewTick);
	prewTick = nowTick;

	while(m_tick >= PRECISION)
	{
		m_tick -= PRECISION;
		if(m_Stop)
			continue;

		{
			auto& timers = m_wheels[0].front();
			if (timers.size() > 0)
			{
				expired(timers);
				timers.clear();
			}
			m_wheels[0].pop_front();
		}

		int i = 0;
		for (auto& wheel = m_wheels.begin(); wheel != m_wheels.end(); wheel++, i++)
		{
			auto next_wheel = wheel;
			if (m_wheels.end() == (++next_wheel))
				break;

			if (wheel->round())
			{
				auto& timers = next_wheel->front();
				if (timers.size() != 0)
				{
					for (auto& it = timers.begin(); it != timers.end(); it++)
					{
						auto slot = getSlot(*it, i);
						(*wheel)[slot].push_back(*it);
					}
					timers.clear();
				}
				next_wheel->pop_front();
			}
		}
	}
}

uint32_t moon::timer_pool::addNewTimer(const timer_context_ptr& t)
{
	t->setEndtime(t->getDuration() + millseconds());
	if (t->getID() == 0)
	{
		//get a not used timer id.
		while (1)
		{
			m_inc++;
			auto iter = m_timers.find(m_inc);
			if (iter == m_timers.end())
			{
				t->setID(m_inc);
				break;
			}
		}
	}
	else
	{
		//timer id  �Ѿ�����
		assert(m_timers.find(t->getID()) == m_timers.end()&&"error!!!");
	}
	m_new.push_back(t);
	return t->getID();
}

// slots:      8bit(notuse) 8bit(wheel3_slot)  8bit(wheel2_slot)  8bit(wheel1_slot)  
uint64_t moon::timer_pool::makeKey(uint32_t id, uint32_t slots)
{
	return ((uint64_t(id) << 32) | slots);
}

void timer_pool::expired(const std::vector<uint64_t>& timers)
{
	for (auto it = timers.begin(); it != timers.end(); it++)
	{
		uint32_t id = ((*it) >> 32) & 0xFFFFFFFF;
		auto iter = m_timers.find(id);
		if (iter != m_timers.end())
		{
			auto tcx = iter->second;
			
			if (!tcx->getRemoved())
			{
				tcx->expired();
				m_timers.erase(id);

				if (tcx->getRepeatTimes() == -1)
				{
					addNewTimer(tcx);
				}
				else if (tcx->getRepeatTimes() > 0)
				{
					tcx->setRepeatTimes(tcx->getRepeatTimes() - 1);
					addNewTimer(tcx);
				}
			}
			else
			{
				m_timers.erase(id);
				continue;
			}	
		}
	}
}

void timer_pool::addTimer(const timer_context_ptr& t)
{
	auto now = millseconds();
	if (t->getEndtime() > now)
	{
		auto diff = t->getEndtime() - now;

		auto offset = diff%PRECISION;
		//У��
		if (offset > 0)
		{
			diff += PRECISION;
		}
		auto slot_count = diff / PRECISION;
		uint64_t key = 0;
		do
		{
			int i = 0; 
			uint32_t slots = 0;
			for (auto& it = m_wheels.begin(); it != m_wheels.end(); it++,i++)
			{
				auto& wheel = *it;
				slot_count += wheel.next_slot();
				uint8_t slot = (slot_count - 1) % (wheel.size());
				slot_count -= slot;
				slots |= slot << (i * 8);
				key = makeKey(t->getID(), slots);
				if (slot_count < wheel.size())
				{
					//printf("timer_pool id %u add to wheel %d slot%d\r\n",t->getID(),  i + 1, slot);
					wheel[slot].push_back(key);
					break;
				}
				slot_count /= wheel.size();
			}
			break;
		} while (0);
		m_timers[t->getID()] = t;
	}
	else
	{
		m_wheels[0].front().push_back(makeKey(t->getID(),0));
		m_timers[t->getID()] = t;
		printf("time out.\r\n");
	}
}

uint8_t moon::timer_pool::getSlot(uint64_t  key, int which_queue)
{
	assert(which_queue < 4);
	return (key >> (which_queue * 8)) & 0xFF;
}

void moon::timer_pool::stopAllTimer()
{
	m_Stop = true;
}

void moon::timer_pool::startAllTimer()
{
	m_Stop = false;
}

int64_t moon::timer_pool::millseconds()
{
	timeval t;
	gettimeofday(&t, nullptr);
	return getms(t);

	//return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

