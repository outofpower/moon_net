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

#include "actor.h"
#include "message_queue.h"
#include "message.h"
#include "actor_manager.h"

namespace moon
{
	struct actor::Imp 
	{
		Imp(module_id moudleid)
			:id(moudleid), mq(moudleid), mgr(nullptr), timerpool(nullptr)
		{
		}

		module_id															id;
		actor_manager*													mgr;
		timer_pool*														    timerpool;
		message_queue<message>								mq;
		std::string																name;
	};

	actor::actor(module_id moudleid)
		:_Imp(std::make_shared<Imp>(moudleid))
	{
	}

	actor::~actor()
	{
	}

	void actor::init(const std::unordered_map<std::string, std::string>& config)
	{

	}

	void actor::broadcast_message(message& msg)
	{
		_Imp->mgr->broadcast_message(get_id(), msg);
	}

	void actor::send_message( module_id receiver,message& msg)
	{
		//if send self message , add to message queue directly.
		if (receiver == get_id())
		{
			msg.set_sender(get_id());
			msg.set_receiver(get_id());
			add_message(msg);
			return;
		}
		_Imp->mgr->send_message(get_id(), receiver, msg);
	}

	const module_id actor::get_id() const
	{
		return _Imp->id;
	}

	const std::string& actor::get_name() const
	{
		return _Imp->name;
	}

	void actor::set_name(const std::string& name)
	{
		_Imp->name = name;
	}

	void actor::set_manager(actor_manager* mgr)
	{
		_Imp->mgr = mgr;
	}

	timer_pool& actor::get_timer_pool()
	{
		return *(_Imp->timerpool);
	}

	void actor::on_enter()
	{
		
	}

	void actor::on_exit()
	{
		
	}

	void actor::update(uint32_t interval)
	{
	
	}

	void actor::add_message(const message& msg)
	{
		_Imp->mq.push_back(msg);
	}

	bool actor::peek_message()
	{
		if (_Imp->mq.size() == 0)
			return false;
		auto& msg = _Imp->mq.front();
		assert(get_id() == msg.get_receiver() || msg.get_receiver() == 0);
		handle_message(msg);
		_Imp->mq.pop_front();
		return _Imp->mq.size() != 0;
	}

	size_t actor::get_mq_size()
	{
		return _Imp->mq.size();
	}

	void actor::set_timer_pool(timer_pool* timerpool)
	{
		_Imp->timerpool = timerpool;
	}
}


