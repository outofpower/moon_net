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
#include <common/singleton.hpp>
#include <common/object_pool.hpp>

#include "actor_define.h"
#include "moon_net_log.h"
namespace moon
{
	//actor �����࣬����actor�Ĵ��������ȣ��Ƴ�
	class actor_manager:public singleton<actor_manager>
	{
	public:
		actor_manager();
		~actor_manager();

		/**
		* ��ʼ��
		*
		* @worker_num �����߳���Ŀ
		*/
		void			init(size_t worker_num);

		/**
		* Ϊ�˿���� actorͨѶ ����machine id,�������ֲ�ͬmachine,��������255��
		* ���ID �ᱣ���� actor ID�ĸ�8λ
		*
		* @id machine id
		*/
		void			set_machine_id(uint8_t id);

		/**
		* ��ȡmachine id
		*
		* @return machine id
		*/
		uint8_t		get_machine_id();

		/**
		* ����actor
		*
		* @config ����actor��������ã��ᴫ�ݸ�actor::init
		*/
		template<class TActor>
		void			create_actor(const std::unordered_map<std::string, std::string>& config);

		/**
		* ����ID�Ƴ�actor
		*
		* @id actor id
		*/
		void			remove_actor(module_id id);

		/**
		* ��ĳ��actor������Ϣ
		*
		* @sender ������id
		* @receiver ������id
		* @msg ��Ϣ����
		*/
		void			send_message(module_id sender, module_id receiver,message& msg);

		/**
		* ������actor�����˷����ߣ��㲥��Ϣ
		*
		* @sender ������id
		* @msg ��Ϣ����
		*/
		void			broadcast_message(module_id sender,message& msg);

		/**
		* ��������worker�߳�
		*
		*/
		void			run();

		/**
		* �ر�����worker�߳�
		*
		*/
		void			stop();

		/**
		* �ڴ��������
		*
		*/
		object_pool<memory_stream, 100, std::mutex>&
			get_memory_stream_pool() { return _mspool; }
	private:
		/**
		* ��ѯ��ȡworker ID
		*/
		uint8_t		get_next_worker(Eget_worker e = worker_roundrobin);

		/**
		* ��actor��ӵ� worker
		* @workerid 
		* @actor_ptr 
		*/
		void			add_to_worker(uint8_t workerid,const actor_ptr& act);

		/**
		* ����actor ID ��ȡ���ڵ� workerID
		* @actorID 
		*/
		uint8_t get_workerID(module_id actorID);

	private:
		std::atomic<uint8_t>													_nextworker;
		std::atomic<uint16_t>													_actorIncID;
		std::vector<worker_ptr>												_workers;
		uint8_t																			_machine_id;

	private:
		object_pool<memory_stream,100,std::mutex>			_mspool;
	};


	template<class TActor>
	void	actor_manager::create_actor(const std::unordered_map<std::string, std::string>& config)
	{
		static_assert(std::is_base_of<actor, TActor>::value, "TActor must inherit actor");

		uint16_t	incID = _actorIncID.fetch_add(1);
		uint8_t		workerID = get_next_worker();
		uint32_t	actorID = 0;
		actorID |= (uint32_t(_machine_id) << 24);//actor ID �� 32-25 bit����machineID
		actorID |= (uint32_t(workerID) << 16);//actor ID �� 24-17 bit����workerID
		actorID |= (incID);

		auto act = std::make_shared<TActor>(module_id::create(actorID));
		act->set_manager(this);
		act->init(config);
		add_to_worker(workerID,act);
	}
};

#define ACTOR_MANAGER moon::actor_manager::Instance()

