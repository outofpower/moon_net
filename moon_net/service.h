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
MOON_BEGIN

DECLARE_SHARED_PTR(socket);
DECLARE_SHARED_PTR(service);

//asio::io_services �ķ�װ �� һ���߳�һ��service
class service
	:public std::enable_shared_from_this<service>
{
public:
	static service_ptr create()
	{
		return std::make_shared<service>();
	}

	service();
	~service(void);

	void run();

	void stop();

	/**
	* ��ȡasio::io_services
	*
	* @return asio::io_services
	*/
	asio::io_service&	get_ioservice();

	/**
	* �����service��� socket
	*
	* @socket 
	*/
	void			add_socket(const socket_ptr& socket);

	/**
	* �Ƴ�socket
	*
	* @socketid socketID
	*/
	void			remove_socket(socket_id socketid);

	/**
	* ��io_services Ͷ�ݳ�ʱ�������
	*
	* @timeout ��ʱ���
	*/
	void			post_check_timeout(uint32_t timeout);

	/**
	* ��ĳ��socket���� ��������
	*
	* @socketID 
	* @buffer_ptr ����
	*/
	void			send(socket_id socketID, const message& msg);

	/**
	* �ر�ĳ��socket����
	*
	* @socketID
	* @state ���ùر�״̬
	*/
	void			close_socket(socket_id socketID, ESocketState state);

	/**
	* ��ȡserviceID
	*
	*/
	uint8_t		get_id() { return _services_id; }

	void			set_id(uint8_t id) {_services_id = id; }

private:
	/**
	* ��ʱ��⺯������ǰֻ���ڲ��ԣ�
	*
	*/
	void			check_timeout(uint32_t timeout);
private:
	asio::io_service												_ioservice;
	asio::io_service::work									_work;
	std::unordered_map<socket_id,socket_ptr> _sockets;
	std::atomic<uint32_t>									_inc_socketid;
	uint8_t															_services_id;
};

MOON_END


