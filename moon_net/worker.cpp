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

#include "worker.h"
#include <common/timer/time.hpp>
#include "actor.h"
#include "actor_manager.h"

namespace moon
{
	worker::worker(actor_manager* mgr)
		:_fps(0), 
		_timer(0),
		_msg_counter(0),
		_worker_id(0),
		_mgr(mgr)
	{

	}


	worker::~worker()
	{

	}


	void worker::run()
	{
		set_interval(10);
		onUpdate = std::bind(&worker::update, this, std::placeholders::_1);
		loop_thread::run();

		NET_LOG.console("worker [{0:d}] run", _worker_id);
	}

	void worker::stop()
	{
		post([this]() {
			for (auto& act : _actors)
			{
				act.second->on_exit();
			}
		});

		while (_events.size() !=0){}
		loop_thread::stop();

		NET_LOG.console("worker [{0:d}] stop", _worker_id);
	}
	 
	void worker::add_actor(const actor_ptr& act)
	{
		post([this, act]() {
			auto& iter = _actors.find(act->get_id());
			assert(iter == _actors.end());
			act->set_timer_pool(&m_timerpool);
			act->on_enter();
			_actors.emplace(act->get_id(), act);

			NET_LOG.console("actor [{0}] [{1:d}] enter", act->get_name().c_str(), act->get_id().value);
		});
	}

	void worker::remove_actor(module_id id)
	{
		post([this, id]() {
			auto& iter = _actors.find(id);
			assert(iter != _actors.end());
			iter->second->on_exit();
			_actors.erase(id);
			NET_LOG.console("actor [{0:d}] exit", id.value);
		});
	}

	void worker::dispatch_message(const message& msg)
	{
		post([this, msg]() {
			auto&t = _actors.find(msg.get_receiver());
			if (t != _actors.end())
			{
				t->second->add_message(msg);
			}
		});
	}



	void worker::broadcast_message(const message& msg)
	{
		post([this, msg]() {
			for (auto& act : _actors)
			{
				if(act.second->get_id() == msg.get_sender())
					continue;
				act.second->add_message(msg);
			}
		});
	}

	int worker::get_msg_fps()
	{
		return _fps.load();
	}

	uint8_t worker::get_id()
	{
		return _worker_id;
	}

	void worker::set_id(uint8_t id)
	{
		_worker_id = id;
	}

	//消息处理
	void worker::update(uint32_t interval)
	{
		m_timerpool.update();

		_timer += interval;
		auto t1 = time::millsecond();
		//1.处理投递的异步事件
		update_events();
		auto t2 = time::millsecond();
		//2.更新各个actors
		for (auto& act : _actors)
		{
			act.second->update(interval);
			//如果actor的消息队列长度 大于0，则把改actor保存到消息处理队列
			if (act.second->get_mq_size() != 0)
			{
				_actors_queue.push_back(act.second.get());
			}	
		}
		auto t3 = time::millsecond();
		//3.循环遍历消息处理队列
		while (_actors_queue.size() != 0)
		{
			auto act = _actors_queue.front();
			_actors_queue.pop_front();

			_msg_counter++;
			//如果该actor还有消息未处理，则把这个actor插到队尾
			if (act->peek_message())
			{		
				_actors_queue.push_back(act);
			}		
		}
		auto t4 = time::millsecond();

		assert(_actors_queue.size() == 0);

		if (_timer > 3000)
		{
			_fps = _msg_counter;
			_msg_counter = 0;
			_timer = 0;

			NET_LOG.console("thread[{0}]:message fps [{1:d}] , event [{2:d} ms], copy[{3:d} ms], work[{4:d} ms]",std::this_thread::get_id(), _fps.load(), t2 - t1, t3 - t2, t4 - t3);
		}
	}
};

