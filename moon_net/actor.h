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
#include <common/noncopyable.hpp>
namespace moon
{
	class actor_manager;

	/**
	* actor的基类.
	*/
	class actor:noncopyable
	{
	public:
		friend class worker;
		friend class actor_manager;
		/**
		* 构造函数
		*
		* @id 一个唯一的ID,默认是个累加值.
		*/
		actor(module_id id);

		virtual							~actor();

		/**
		* 向所有actor(除了当前actor)广播消息
		*
		* @msg 消息数据
		*/
		void								broadcast_message(message& msg);

		/**
		* 向某个actor发送消息
		*
		* @msg 消息数据
		*/
		void								send_message(module_id receiver,message& msg);

		/**
		* 获取这个actor的 ID
		*
		* @return actorID
		*/
		const module_id			get_id() const;

		/**
		* 获取这个actor的name
		*
		* @return actor name
		*/
		const std::string&		get_name() const;

		/**
		* 获取定时器管理器
		*
		* @return 定时器管理器
		*/
		timer_pool&				get_timer_pool();
	protected:
		/**
		* 根据配置初始化这个actor,应该重写这个函数,初始化派生的actor
		*
		* @config key-value 配置
		*/
		virtual void					init(const std::unordered_map<std::string, std::string>& config);

		/**
		* 设置actor的name
		*
		* @name 
		*/
		void								set_name(const std::string& name);

		/**
		* 当actor添加进worker thread 时会调用此函数
		*
		*/
		virtual void					on_enter();

		/**
		* 当actor从worker thread移除时会调用此函数
		*
		*/
		virtual void					on_exit();

		/**
		* 消息处理函数，应该重写此函数 来处理消息
		*
		*	@msg  消息
		*/
		virtual void					handle_message(const message& msg) = 0;

		/**
		* 把消息添加进 actor的消息队列
		*
		*	@msg  消息
		*/
		void								add_message(const message& msg);
		
		/**
		* 获取当前actor消息队列的大小
		*
		*	@return  消息队列的大小
		*/
		size_t							get_mq_size();

		/**
		* 消息处理驱动函数，由worker thread调用
		*
		*	@interval 循环间隔
		*/
		virtual void					update(uint32_t interval);
	private:
		void								set_manager(actor_manager* mgr);

		void								set_timer_pool(timer_pool* tmgr);
		//check message queue,pop a message,if message queue size >0,return true else return false

		/**
		* 检查actor的消息队列长度，如果大于0 返回true 否则返回false
		*
		*	@return 
		*/
		bool								peek_message();
	private:
		struct Imp;
		std::shared_ptr<Imp>											_Imp;
	};
};

