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

#include "actor_manager.h"
#include "worker.h"

namespace moon
{
	actor_manager::actor_manager()
		:_machine_id(1), _nextworker(0), _actorIncID(1)
	{

	}

	actor_manager::~actor_manager()
	{

	}

	void actor_manager::init(size_t worker_num)
	{
		for (uint8_t i = 0; i != worker_num; i++)
		{
			auto& wk = std::make_shared<worker>(this);
			_workers.push_back(wk);
			wk->set_id(i);
		}

		console()->info("actor_manager initialized with {0:d} worker thread.", worker_num);
	}

	void actor_manager::set_machine_id(uint8_t id)
	{
		_machine_id = id;
	}

	uint8_t actor_manager::get_machine_id()
	{
		return _machine_id;
	}

	void actor_manager::remove_actor(module_id actorID)
	{
		uint8_t workerID = get_workerID(actorID);
		if (workerID < _workers.size())
		{		
			_workers[workerID]->remove_actor(actorID);
		}
	}

	void actor_manager::send_message(module_id sender, module_id receiver,message& msg)
	{
		assert(msg.get_type() != EMessageType::Unknown);
		msg.set_sender(sender);
		msg.set_receiver(receiver);
		uint8_t workerID = get_workerID(receiver);
		if (workerID < _workers.size())
		{
			_workers[workerID]->dispatch_message(msg);
		}
	}

	void actor_manager::broadcast_message(module_id sender,message& msg)
	{
		assert(msg.get_type() != EMessageType::Unknown);
		msg.set_sender(sender);
		msg.set_receiver(module_id::create(0));
		for (auto& w : _workers)
		{
			w->broadcast_message(msg);
		}
	}

	void actor_manager::run()
	{
		console()->info("actor_manager start");
		for (auto& w : _workers)
		{
			w->run();
		}
	}

	void actor_manager::stop()
	{
		for (auto& w : _workers)
		{
			w->stop();
		}
		console()->info("actor_manager stop");
	}

	uint8_t actor_manager::get_next_worker(Eget_worker e /*= worker_roundrobin*/)
	{
		if (e == worker_roundrobin)
		{
			if (_nextworker < _workers.size())
			{
				uint8_t tmp = _nextworker.load();
				_nextworker++;
				if (_nextworker == _workers.size())
				{
					_nextworker = 0;
				}
				return tmp;
			}
		}
		else if (e == worker_minfps)
		{
			uint32_t minfps = -1;

			worker_ptr ret = nullptr;
			for (auto& wk : _workers)
			{
				uint32_t f = wk->get_msg_fps();
				if (f < minfps)
				{
					ret = wk;
				}
			}
			return ret->get_id();
		}
		return 0;
	}

	void actor_manager::add_to_worker(uint8_t workerid,const actor_ptr& act)
	{
		for (auto& wk : _workers)
		{
			if (wk->get_id() == workerid)
			{
				wk->add_actor(act);
				return;
			}
		}
	}

	uint8_t actor_manager::get_workerID(module_id actorID)
	{
		return ((actorID.value >> 16) & 0xFF);
	}
};

