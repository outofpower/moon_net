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
#include "actor.h"

namespace moon
{
	//��������ͨѶ���ܵ�actor
	class broker :public actor
	{
	public:
		broker(module_id id);

		virtual ~broker();

		virtual void	init(const std::unordered_map<std::string, std::string>& config){}

		virtual void	on_enter() override;

		virtual void	on_exit() override;

		virtual void	handle_message(const message& msg) = 0;
		
		virtual void	update(uint32_t interval);

		void				send_socket_message(socket_id sockid,const message& msg);

	protected:
		/**
		* ��ʼ������
		*
		* @thread_num �����߳���
		*/
		void				init_net(int thread_num);
		
		/**
		* ��������ĵ�ַ
		*
		* @ip 
		* @port
		*/
		bool				listen(const std::string& ip, const std::string& port);

		/**
		* �첽���ӷ������������Ӷ����
		*
		* @ip
		* @port
		*/
		void				connect(const std::string& ip, const std::string& port);

		/**
		* �յ�������Ϣʱ����ô˺���
		*
		* @msg ������Ϣ
		*/
		void				on_net_message(const message& msg);

		/**
		* ǿ�ƹر�һ����������
		*
		* @sockid
		*/
		void				force_close_socket(socket_id sockid);
	protected:
		struct  Imp;
		std::shared_ptr<Imp>																	   _Imp;
	};
};


