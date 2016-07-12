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

namespace moon
{
	union sender
	{
		sender() {}
		sender(const sender& other) :_sender(other._sender), _sockid(_sockid) {}

		module_id							_sender;
		socket_id								_sockid;
	};

	class message::Imp
	{
	public:
		Imp(const buffer_ptr& data)
			:_data(data), _type(EMessageType::Unknown)
		{

		}
		sender										_sender;
		module_id								_receiver;
		EMessageType							_type;
		buffer_ptr								_data;
		mutable std::vector<uint8_t>				_userdata;
	};

	message::message(const buffer_ptr& data)
		:_imp(std::make_shared<Imp>(data))
	{

	}

	message::message(const message& msg)
		:_imp(msg._imp)
	{

	}

	message::message(message&& msg)
		: _imp(std::move(msg._imp))
	{

	}

	message::~message()
	{
		
	}

	moon::module_id message::get_sender() const
	{
		return  _imp->_sender._sender;
	}

	moon::socket_id message::get_socket_id() const
	{
		return _imp->_sender._sockid;
	}

	void message::set_sender(module_id sder)
	{
		_imp->_sender._sender = sder;
	}

	void message::set_sender(socket_id sockid)
	{
		_imp->_sender._sockid = sockid;
	}

	void message::set_userdata(uint8_t* data, size_t size) const
	{
		if (_imp->_userdata.size() < size)
		{
			_imp->_userdata.resize(size);
		}
		std::copy(data, data +size, _imp->_userdata.begin());
	}

	const uint8_t* message::get_userdata(size_t size) const
	{
		if(_imp->_userdata.size() >= size)
			return _imp->_userdata.data();
		return nullptr;
	}

	const buffer_ptr& message::msg_data() const
	{
		return _imp->_data;
	}

	const uint8_t* message::data() const
	{
		if (_imp->_data != nullptr)
			return _imp->_data->data();

		return nullptr;
	}

	size_t message::size() const
	{
		if (_imp->_data != nullptr)
			return _imp->_data->size();
		return 0;
	}

	void message::set_receiver(module_id receiver)
	{
		_imp->_receiver = receiver;
	}

	module_id message::get_receiver() const
	{
		return _imp->_receiver;
	}

	void message::set_type(EMessageType type)
	{
		_imp->_type = type;
	}

	moon::EMessageType message::get_type() const
	{
		return _imp->_type;
	}

}

