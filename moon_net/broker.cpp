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

#include "broker.h"
#include <common/blocking_queue.hpp>
#include "message.h"
#include "tcp_frame.h"

namespace moon
{
	struct  broker::Imp
	{
		Imp(const net_message_hander& hander, uint8_t  thread_num)
			:_net(hander,thread_num)
		{

		}

		blocking_queue<message>													_net_msg;
		tcp_frame																				_net;
	};

	broker::broker(module_id id)
		:actor(id)
	{
		
	}

	broker::~broker()
	{

	}

	void broker::on_enter()
	{
		actor::on_enter();
		_Imp->_net.run();
	}

	void broker::on_exit()
	{
		actor::on_exit();
		_Imp->_net.stop();
	}

	void broker::init_net(int thread_num)
	{
		_Imp = std::make_shared<Imp>(FUNC_BIND1(broker::on_net_message,this),  thread_num);
	}

	bool broker::listen(const std::string& ip, const std::string& port)
	{
		if (ip.size() == 0 || port.size() == 0)
		{
			printf("waring listen ip ,port is nul\r\n");
			return false;
		}

		_Imp->_net.listen(ip, port, get_id());

		return (_Imp->_net.getErrorCode() == 0);
	}

	void broker::connect(const std::string& ip, const std::string& port)
	{
		_Imp->_net.connect(ip, port, get_id());
	}

	void broker::update(uint32_t interval)
	{
		auto msgs = _Imp->_net_msg.Move();
		for (auto& msg : msgs)
		{
			add_message(msg);
		}
	}

	void broker::on_net_message(const message& msg)
	{
		_Imp->_net_msg.push_back(msg);
	}

	void broker::send_socket_message(socket_id sockid, const buffer_ptr& data)
	{
		_Imp->_net.send(sockid, data);
	}

	void broker::force_close_socket(socket_id sockid)
	{
		_Imp->_net.close_socket(sockid,  ESocketState::Force_Closing);
	}

}