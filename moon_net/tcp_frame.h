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
	/**
	* tcp_frame 实现了网络库操作接口。 tcp_frame 对象本身是非线程安全的。
	*/
	class tcp_frame
	{
	public:
		/**
		* tcp_frame 构造函数
		* @handler 网络事件回掉（connect, recevie, close)
		* @net_thread_num 网络线程数量
		* 注意：如果开启了多个网络线程，那么handler 回掉函数是非线程安全的。参见
		*	broker::on_net_message ，里面使用了加锁队列
		*/
		tcp_frame(const net_message_hander& handler, uint8_t net_thread_num = 1);
		~tcp_frame();

		/**
		* 监听某个端口
		* @listenAddress ip地址或者域名
		* @listenPort 端口
		*/
		void							listen(const std::string& ip, const std::string& port, module_id moduleid);

		/**
		* 异步连接某个端口
		* @ip ip地址或者域名
		* @port 端口
		* @moduleid 模块标识符,一般情况下网络库是作为某个模块的一部分 参考 broker的实现。
		*	网络事件会以消息（message）的形式，插入到模块的消息队列中.
		*/
		void							async_connect(const std::string& ip, const std::string& port, module_id moduleid);

		/**
		* 同步连接某个端口
		* @ip ip地址或者域名
		* @port 端口
		* @moduleid 模块标识符,一般情况下网络库是作为某个模块的一部分 参考 broker的实现。
		*	网络事件会以消息（message）的形式，插入到模块的消息队列中.
		* @return 返回链接的 socketID, 成功 socketID.value != 0, 失败socketID.value = 0
		*/
		socket_id					sync_connect(const std::string& ip, const std::string& port, module_id moduleid);

		/**
		* 向某个链接发送数据, 这个函数是线程安全的
		* @sockid 连接标识
		* @data 数据
		*/
		void							send(socket_id sockid, const buffer_ptr& data);

		/**
		* 关闭一个链接
		* @sockid 连接标识
		* @state 给链接设置一个状态，表明为什么关闭（例如 超时，发送非法数据）
		*/
		void							close_socket(socket_id sockid, ESocketState state);

		/**
		* 启动网络库
		*/
		void							run();

		/**
		* 停止网络库
		*/
		void							stop();

		/**
		* 获取错误码
		*/
		int							getErrorCode();

		/**
		* 获取错误信息
		*/
		std::string					getErrorMessage();

		/**
		* 设置管理链接的超时检测
		* @timeout 超时时间 ，单位 ms
		* @checkInterval 超时检测间隔，单位 ms
		*/
		void							setTimeout(uint32_t timeout, uint32_t checkInterval);
	protected:
		/**
		* 投递异步accept,接受网络连接
		*/
		void							postAccept();

		/**
		* 超时检测线程函数
		*/
		void							checkTimeOut(uint32_t interval);
	protected:
		struct Imp;
		std::shared_ptr<Imp> _Imp;
		net_message_hander  _hander;
	};
};
