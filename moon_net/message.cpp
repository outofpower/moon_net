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

#include "message.h"
#include "actor_manager.h"

namespace moon
{
	message::message(size_t capacity, size_t headreserved)
		:_type( EMessageType::Unknown)
	{
		_msgdata = ACTOR_MANAGER.get_memory_stream_pool().create(capacity, headreserved);
	}

	message::~message()
	{
		
	}

	moon::module_id message::get_sender() const
	{
		return  _sender._sender;
	}

	moon::socket_id message::get_socket_id() const
	{
		return _sender._sockid;
	}

	void message::set_sender(module_id sder)
	{
		_sender._sender = sder;
	}

	void message::set_sender(socket_id sockid)
	{
		_sender._sockid = sockid;
	}

	void message::set_userdata(uint8_t* data, size_t size) const
	{
		if (_userdata.size() < size)
		{
			_userdata.resize(size);
		}
		std::copy(data, data +size, _userdata.begin());
	}

	const uint8_t* message::get_userdata(size_t size) const
	{
		if(_userdata.size() >= size)
			return _userdata.data();
		return nullptr;
	}

	void message::set_receiver(module_id receiver)
	{
		_receiver = receiver;
	}

	module_id message::get_receiver() const
	{
		return _receiver;
	}

	void message::set_type(EMessageType type)
	{
		_type = type;
	}

	moon::EMessageType message::get_type() const
	{
		return _type;
	}
}

