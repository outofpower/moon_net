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
#include <common/loop_thread.hpp>
#include "actor_define.h"
#include "async_event.h"
#include "message.h"

namespace moon
{
	class actor_manager;

	//工作线程
	class worker:public loop_thread,public async_event, noncopyable
	{
	public:
		worker(actor_manager* mgr);

		~worker();

		/**
		* 启动工作线程
		*
		*/
		void run() override;

		/**
		* 停止工作线程
		*
		*/
		void stop() override;

		/**
		* 向工作线程添加actor
		*
		*/
		void add_actor(const actor_ptr& act);

		/**
		* 移除actor
		*
		*/
		void remove_actor(module_id id);

		/**
		* 消息分发
		*/
		void dispatch_message(const message&msg);

		/**
		* 向该worker中的所有actor广播消息
		*/
		void broadcast_message(const message& msg);

		int	get_msg_fps();

		uint8_t	get_id();

		void set_id(uint8_t id);
	private:
		void update(uint32_t interval);

	private:
		uint8_t																_worker_id;
		std::unordered_map<module_id, actor_ptr>	_actors;
		//消息处理队列
		std::deque<actor*>											_actors_queue;
		actor_manager*												_mgr;

		std::atomic<int>												_fps;
		uint32_t															_msg_counter;
		uint32_t															_timer;

		timer_pool														m_timerpool;
	};
};


