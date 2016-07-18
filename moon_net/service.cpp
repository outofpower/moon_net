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

#include  "service.h"
#include  "socket.h"
MOON_BEGIN

service::service()
	:_work(_ioservice), _inc_socketid(0), _services_id(-1)
{
}

service::~service(void)
{
	for (auto& sock : _sockets)
	{
		sock.second->close(ESocketState::Ok);
	}
}

asio::io_service& service::get_ioservice()
{
	return _ioservice;
}

void service::add_socket(const socket_ptr& socket)
{
	_ioservice.post([this,socket]() {
		auto tmp = ++_inc_socketid;
		socket_id sockid;
		sockid.value = uint32_t(_services_id) << 24 | tmp;
		socket->setsocket_id(sockid);
		_sockets[sockid] = socket;
		socket->start();
	});
}

void service::remove_socket(socket_id socketid)
{
	_ioservice.post([this,socketid]() {
		if (_sockets.find(socketid) != _sockets.end())
		{
			_sockets.erase(socketid);
		}
	});
}

void service::post_check_timeout(uint32_t timeout)
{
	_ioservice.post([this, timeout]()
	{
		check_timeout(timeout);
	});
}

void service::send(socket_id socketID, const buffer_ptr & data)
{
	_ioservice.post([this,socketID, data]()
	{
		auto& iter = _sockets.find(socketID);
		if (iter != _sockets.end())
		{
			iter->second->send(data);
		}
	});
}

void service::check_timeout(uint32_t timeout)
{
	auto curSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	
	for (auto& iter : _sockets)
	{
		int64_t diff = curSec - iter.second->getlastRecvTime();
		if (diff >= timeout)
		{
			//投递socket连接关闭请求，此操作是异步的
			iter.second->close(ESocketState::Timeout);
		}
	}
}

void service::run()
{
	_ioservice.run();
}

void service::stop()
{
	_ioservice.stop();
}

void service::close_socket(socket_id socketID, ESocketState state)
{
	_ioservice.post([this, socketID, state]()
	{
		auto& iter = _sockets.find(socketID);
		if (iter != _sockets.end())
		{
			iter->second->close(state);
		}
	});
}


MOON_END

