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
#include "message.h"

namespace moon
{
	//asio::socket �ķ�װ
	class service;
	class socket :public std::enable_shared_from_this<socket>, private asio::noncopyable
	{
		static const int32_t			IO_BUFFER_SIZE = 8192;
	public:
		explicit socket(net_message_hander& handler,service& ser);
		~socket(void);

		/**
		* ���ӳɹ�����ô˺�������һЩ��ʼ������
		*
		* @return ��ʼ���ɹ�����true��������ӵ�service�У��� ���򷵻�false
		*/
		bool											start();

		/**
		* ǿ�ƹرո�socket����
		*
		* @state ����һ���ر�״̬
		*/
		void											close(ESocketState state);

		/**
		* ���socket���ӷ�������
		*
		*/
		void											send(const message& data);

		/**
		* ��ȡasio::ip::tcp::socket
		*
		*/
		asio::ip::tcp::socket&				get_socket() { return _socket; };

		/**
		* ��ȡ���������
		*
		*/
		asio::error_code						get_errorcode() { return _errorCode; };

		/**
		* ��ȡsocket���ӵ�Զ�̵�ַ
		*
		*/
		std::string									get_remoteaddress();

		/**
		* ��ȡ������Ϣ
		*
		*/
		std::string									get_error_message() { return _errorCode.message(); }

		/**
		* �������������ӵ�״̬
		*
		*/
		bool											checkState();
		//socket ��8λ Ϊ����services��id
		ADD_READWRITE(socket_id, _sockid, socket_id);
		ADD_READWRITE(module_id, _module_id, module_id);
		ADD_READWRITE(ESocketState, _state, state);
		ADD_READONLY(int64_t, _lastRecvTime, lastRecvTime);
	private:
		/**
		* Ͷ���첽������
		*
		*/
		void											postRead();

		/**
		* Ͷ���첽д����
		*
		*/
		void											postSend();

		/**
		* ��ȡ��ɻص�
		*
		*/
		void											handleRead(const asio::error_code& e, std::size_t bytes_transferred);

		/**
		* д��ɻص�
		*
		*/
		void											handleSend(const asio::error_code& e, std::size_t bytes_transferred);
		void											onClose();
	private:
		service&									_service;

		asio::ip::tcp::socket					_socket;

		asio::ip::tcp::endpoint				_remoteEndPoint;
		//�첽���ջ�����
		uint8_t										_recvBuffer[IO_BUFFER_SIZE];
		//������Ϣ������
		memory_stream						_recvms;
		//���ͻ�����
		memory_stream						_sendms;
		//������Ϣ���Ͷ���
		std::deque<message>			_sendQueue;
		//�Ƿ����ڷ���
		bool											_isSending;
		//�������
		asio::error_code						_errorCode;
		//��Ϣ����������
		net_message_hander&			_handler;
	};
};





