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
#include "moon_net_log.h"
namespace moon
{
	//actor 管理类，负责actor的创建，调度，移除
	class actor_manager
	{
	public:
		actor_manager();
		~actor_manager();

		/**
		* 初始化
		*
		* @worker_num 工作线程数目
		*/
		void			init(size_t worker_num);

		/**
		* 为了跨进程 actor通讯 设置machine id,用于区分不同machine,最多可以有255个
		* 这个ID 会保存在 actor ID的高8位
		*
		* @id machine id
		*/
		void			set_machine_id(uint8_t id);

		/**
		* 获取machine id
		*
		* @return machine id
		*/
		uint8_t		get_machine_id();

		/**
		* 创建actor
		*
		* @config 创建actor所需的配置，会传递给actor::init
		*/
		template<class TActor>
		void			create_actor(const std::unordered_map<std::string, std::string>& config);

		/**
		* 根据ID移除actor
		*
		* @id actor id
		*/
		void			remove_actor(module_id id);

		/**
		* 向某个actor发送消息
		*
		* @sender 发送者id
		* @receiver 接收者id
		* @msg 消息内容
		*/
		void			send_message(module_id sender, module_id receiver,message& msg);

		/**
		* 向所有actor（除了发送者）广播消息
		*
		* @sender 发送者id
		* @msg 消息内容
		*/
		void			broadcast_message(module_id sender,message& msg);

		/**
		* 启动所有worker线程
		*
		*/
		void			run();

		/**
		* 关闭所有worker线程
		*
		*/
		void			stop();
	private:
		/**
		* 轮询获取worker ID
		*/
		uint8_t		get_next_worker(Eget_worker e = worker_roundrobin);

		/**
		* 把actor添加到 worker
		* @workerid 
		* @actor_ptr 
		*/
		void			add_to_worker(uint8_t workerid,const actor_ptr& act);

		/**
		* 根据actor ID 获取所在的 workerID
		* @actorID 
		*/
		uint8_t get_workerID(module_id actorID);

	private:
		moon_net_log																m_Log;
		std::atomic<uint8_t>													_nextworker;
		std::atomic<uint16_t>													_actorIncID;
		std::vector<worker_ptr>												_workers;
		uint8_t																			_machine_id;
	};


	template<class TActor>
	void	actor_manager::create_actor(const std::unordered_map<std::string, std::string>& config)
	{
		static_assert(std::is_base_of<actor, TActor>::value, "TActor must inherit actor");

		uint16_t	incID = _actorIncID.fetch_add(1);
		uint8_t		workerID = get_next_worker();
		uint32_t	actorID = 0;
		actorID |= (uint32_t(_machine_id) << 24);
		actorID |= (uint32_t(workerID) << 16);
		actorID |= (incID);

		auto act = std::make_shared<TActor>(module_id::create(actorID));
		act->set_manager(this);
		act->init(config);
		add_to_worker(workerID,act);
	}
};



