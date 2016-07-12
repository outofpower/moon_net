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
	//带有网络通讯功能的actor
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

		void				send_socket_message(socket_id sockid, const buffer_ptr& data);

	protected:
		/**
		* 初始化网络
		*
		* @thread_num 网络线程数
		*/
		void				init_net(int thread_num);
		
		/**
		* 网络监听的地址
		*
		* @ip 
		* @port
		*/
		bool				listen(const std::string& ip, const std::string& port);

		/**
		* 异步连接服务器（可连接多个）
		*
		* @ip
		* @port
		*/
		void				connect(const std::string& ip, const std::string& port);

		/**
		* 收到网络消息时会调用此函数
		*
		* @msg 网络消息
		*/
		void				on_net_message(const message& msg);

		/**
		* 强制关闭一个网络连接
		*
		* @sockid
		*/
		void				force_close_socket(socket_id sockid);
	protected:
		struct  Imp;
		std::shared_ptr<Imp>																	   _Imp;
	};
};


