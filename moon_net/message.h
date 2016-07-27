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
#include "actor_define.h"

namespace moon
{
	//actor 消息
	class message
	{
	public:
		message(size_t capacity = 64, size_t headreserved = 0);

		message(const message& msg) = default;

		message(message&& msg) = default;

		~message();

		/**
		* 获取消息的发送者ID，一个消息要么是actor之间通讯的消息，要么是网络消息
		* 参考 get_socket_id
		*/
		module_id				get_sender() const;

		/**
		* 获取消息的socketID ,参考get_sender
		*/
		socket_id					get_socket_id() const;
	
		/**
		* 设置消息的发送者ID,代表这个消息是 actor间通讯的消息
		*/
		void							set_sender(module_id sender);

		/**
		* 设置消息的发送者ID,代表这个消息是 网络消息
		*/
		void							set_sender(socket_id sender);

		/**
		* 设置消息的接收者ID
		*/
		void							set_receiver(module_id sender);

		module_id				get_receiver() const;

		/**
		* 设置用户自定义数据
		*/
		void							set_userdata(uint8_t* data, size_t size) const;
	
		/**
		* 获取用户自定义数据
		*/
		const uint8_t*			get_userdata(size_t size) const;

		/**
		* 设置消息类型，参见 EMessageType
		*/
		void							set_type(EMessageType type);

		/**
		* 获取消息类型，参见 EMessageType
		*/
		EMessageType		   get_type() const;

		operator memory_stream*()
		{
			return _msgdata.get();
		}

		const uint8_t* data() const
		{
			return _msgdata->data();
		}

		size_t size() const
		{
			return _msgdata->size();
		}
	protected:
		union sender
		{
			sender() {}
			sender(const sender& other) :_sender(other._sender), _sockid(_sockid) {}

			module_id							_sender;
			socket_id								_sockid;
		};

		sender													_sender;
		module_id											_receiver;
		EMessageType										_type;
		mutable std::vector<uint8_t>				_userdata;
		memory_stream_ptr						    _msgdata;
	};
};


