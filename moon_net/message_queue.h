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
	//actor消息队列
	template<class TMsg>
	class message_queue
	{
	public:
		/**
		* 构造函数
		*
		* @id 消息队列所属的actorID
		*/
		message_queue(module_id id)
			:_module_id(id)
		{

		}

		~message_queue()
		{

		}

		/**
		* 获取队头的消息
		*
		* @return 
		*/
		TMsg& front()
		{
			assert(size() > 0);
			return _msg_queue.front();
		}

		/**
		* 移除队头的消息
		*
		*/
		void pop_front()
		{
			assert(size() > 0);
			_msg_queue.pop_front();
		}

		/**
		* 向队尾添加消息
		*
		* @msg 
		*/
		void push_back(const TMsg& msg)
		{
			_msg_queue.push_back(msg);
		}

		/**
		* 向队尾添加消息（right value)
		*
		* @msg
		*/
		void emplace_back(TMsg&& msg)
		{
			_msg_queue.emplace_back(std::forward<TMsg>(msg));
		}
		
		/**
		* 获取消息队列所属的actor ID
		*
		* @return actor ID
		*/
		module_id get_module_id()
		{
			return _module_id;
		}

		/**
		* 获取消息队列长度
		*
		*/
		size_t size()
		{
			return _msg_queue.size();
		}

	protected:
		std::deque<TMsg>				_msg_queue;
		module_id							_module_id;
	};
};


