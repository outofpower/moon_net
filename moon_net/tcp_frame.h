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
	* tcp_frame ʵ�������������ӿڡ� tcp_frame �������Ƿ��̰߳�ȫ�ġ�
	*/
	class tcp_frame
	{
	public:
		/**
		* tcp_frame ���캯��
		* @handler �����¼��ص���connect, recevie, close)
		* @net_thread_num �����߳�����
		* ע�⣺��������˶�������̣߳���ôhandler �ص������Ƿ��̰߳�ȫ�ġ��μ�
		*	broker::on_net_message ������ʹ���˼�������
		*/
		tcp_frame(const net_message_hander& handler, uint8_t net_thread_num = 1);
		~tcp_frame();

		/**
		* ����ĳ���˿�
		* @listenAddress ip��ַ��������
		* @listenPort �˿�
		*/
		void							listen(const std::string& ip, const std::string& port, module_id moduleid);

		/**
		* �첽����ĳ���˿�
		* @ip ip��ַ��������
		* @port �˿�
		* @moduleid ģ���ʶ��,һ����������������Ϊĳ��ģ���һ���� �ο� broker��ʵ�֡�
		*	�����¼�������Ϣ��message������ʽ�����뵽ģ�����Ϣ������.
		*/
		void							async_connect(const std::string& ip, const std::string& port, module_id moduleid);

		/**
		* ͬ������ĳ���˿�
		* @ip ip��ַ��������
		* @port �˿�
		* @moduleid ģ���ʶ��,һ����������������Ϊĳ��ģ���һ���� �ο� broker��ʵ�֡�
		*	�����¼�������Ϣ��message������ʽ�����뵽ģ�����Ϣ������.
		* @return �������ӵ� socketID, �ɹ� socketID.value != 0, ʧ��socketID.value = 0
		*/
		socket_id					sync_connect(const std::string& ip, const std::string& port, module_id moduleid);

		/**
		* ��ĳ�����ӷ�������, ����������̰߳�ȫ��
		* @sockid ���ӱ�ʶ
		* @data ����
		*/
		void							send(socket_id sockid, const message& msg);

		/**
		* �ر�һ������
		* @sockid ���ӱ�ʶ
		* @state ����������һ��״̬������Ϊʲô�رգ����� ��ʱ�����ͷǷ����ݣ�
		*/
		void							close_socket(socket_id sockid, ESocketState state);

		/**
		* ���������
		*/
		void							run();

		/**
		* ֹͣ�����
		*/
		void							stop();

		/**
		* ��ȡ������
		*/
		int							getErrorCode();

		/**
		* ��ȡ������Ϣ
		*/
		std::string					getErrorMessage();

		/**
		* ���ù������ӵĳ�ʱ���
		* @timeout ��ʱʱ�� ����λ ms
		* @checkInterval ��ʱ���������λ ms
		*/
		void							setTimeout(uint32_t timeout, uint32_t checkInterval);
	protected:
		/**
		* Ͷ���첽accept,������������
		*/
		void							postAccept();

		/**
		* ��ʱ����̺߳���
		*/
		void							checkTimeOut(uint32_t interval);
	protected:
		struct Imp;
		std::shared_ptr<Imp> _Imp;
		net_message_hander  _hander;
	};
};
