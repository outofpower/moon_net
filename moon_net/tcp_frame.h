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
	class tcp_frame
	{
	public:
		tcp_frame(const net_message_hander& handler,  uint8_t net_thread_num = 1);
		~tcp_frame();
		//监听某个端口
		void							listen(const std::string& listenAddress, const std::string& listenPort, module_id moduleid);
		//连接远程服务器
		void							connect(const std::string& ip, const std::string& port, module_id moduleid);
		//发送数据
		void							send(socket_id sockid, const buffer_ptr& data);

		void							close_socket(socket_id sockid, ESocketState state);
		//开启服务器
		void							run();
		//停止服务器
		void							stop();
		//获取错误信息
		int							getErrorCode();

		std::string					getErrorMessage();
		//设置超时检测（毫秒）
		void							setTimeout(uint32_t timeout, uint32_t checkInterval);
	protected:
		// 投递异步accept
		void							postAccept();
		// 超时检测线程函数
		void							checkTimeOut(uint32_t interval);
	protected:
		struct Imp;
		std::shared_ptr<Imp> _Imp;

		net_message_hander  _hander;
	};
};
