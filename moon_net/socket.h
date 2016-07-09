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
#include <asio/asio.hpp>

namespace moon
{
	//asio::socket 的封装
	class service;
	class socket :public std::enable_shared_from_this<socket>, private asio::noncopyable
	{
		static const int32_t			IO_BUFFER_SIZE = 8192;
	public:
		explicit socket(net_message_hander& handler,service& ser);
		~socket(void);

		/**
		* 连接成功后调用此函数，做一些初始化工作
		*
		* @return 初始化成功返回true（将会添加到service中）， 否则返回false
		*/
		bool											start();

		/**
		* 强制关闭该socket连接
		*
		* @state 设置一个关闭状态
		*/
		void											close(ESocketState state);

		/**
		* 向该socket连接发送数据
		*
		*/
		void											send(const buffer_ptr& pkt);

		/**
		* 获取asio::ip::tcp::socket
		*
		*/
		asio::ip::tcp::socket&				get_socket() { return _socket; };

		/**
		* 获取网络错误码
		*
		*/
		asio::error_code						get_errorcode() { return _errorCode; };

		/**
		* 获取socket连接的远程地址
		*
		*/
		std::string									get_remoteaddress();

		/**
		* 获取错误信息
		*
		*/
		std::string									get_error_message() { return _errorCode.message(); }

		/**
		* 检测这个网络连接的状态
		*
		*/
		bool											checkState();
		//socket 高8位 为所在services的id
		ADD_READWRITE(socket_id, _sockid, socket_id);
		ADD_READWRITE(module_id, _module_id, module_id);
		ADD_READWRITE(ESocketState, _state, state);
		ADD_READONLY(int64_t, _lastRecvTime, lastRecvTime);
	private:
		/**
		* 投递异步读请求
		*
		*/
		void											postRead();

		/**
		* 投递异步写请求
		*
		*/
		void											postSend();

		/**
		* 读取完成回掉
		*
		*/
		void											handleRead(const asio::error_code& e, std::size_t bytes_transferred);

		/**
		* 写完成回掉
		*
		*/
		void											handleSend(const asio::error_code& e, std::size_t bytes_transferred);
		void											onClose();
	private:
		service&									_service;

		asio::ip::tcp::socket					_socket;

		asio::ip::tcp::endpoint				_remoteEndPoint;
		//异步接收缓冲区
		uint8_t										_recvBuffer[IO_BUFFER_SIZE];
		//接收消息缓冲区
		buffer										_recvBuf;
		//发送缓冲区
		buffer										_sendBuf;
		//发送消息发送队列
		std::deque<buffer_ptr>			_sendQueue;
		//是否正在发送
		bool											_isSending;
		//网络错误
		asio::error_code						_errorCode;
		//消息处理函数对象
		net_message_hander&			_handler;
	};
};





