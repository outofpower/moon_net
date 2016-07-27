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
#include <common/moon_define.hpp>
#include <common/timer/timer_pool.h>
#include <common/noncopyable.hpp>
#include <common/memory_stream.hpp>
#include <common/binary_writer.hpp>
#include <common/binary_reader.hpp>
#include <common/object_pool.hpp>

#include "socket_id.h"
#include "module_id.h"

namespace moon
{
	class actor;
	class moon_server;
	class message;
	class worker;

	using actor_ptr		= std::shared_ptr<actor>;
	using actor_wptr		= std::weak_ptr<actor>;
	using worker_ptr		= std::shared_ptr<worker>;

	using net_message_hander = std::function<void(const message&)>;

	enum Eget_worker
	{
		worker_roundrobin,
		worker_minfps	
	};

	//actor��Ϣ����
	enum class EMessageType
	{
		Unknown,
		SocketData,//��������
		SocketConnect,//����������Ϣ
		SocketClose,//����Ͽ���Ϣ
		SocketTypeEnd,
		ActorData,//actor����
		ToClient//���͸��ͻ��˵�����
	};

	enum  class ESocketState
	{
		Ok,
		Created,
		Accepted,
		Connected,
		Timeout,
		ClientClose,
		Closed,
		Force_Closing
	};

	typedef uint16_t msg_size_t;

	//Ĭ�ϳ�ʱ���(����)
#define DEFAULT_TIMEOUT_INTERVAL     1000
	//Ĭ�ϳ�ʱ����� �����룩
#define DEFAULT_TIMEOUTCHECK_INTERVAL 2000
	//�����Ϣ����
#define MAX_MSG_SIZE msg_size_t(-1)
};

