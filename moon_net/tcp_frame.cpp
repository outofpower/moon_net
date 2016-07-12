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

#include "tcp_frame.h"
#include <common/loop_thread.hpp>
#include "socket.h"
#include "service_pool.h"
#include "moon_net_log.h"

MOON_BEGIN

struct tcp_frame::Imp
{
	Imp(uint8_t net_thread_num)
		:servicepool(net_thread_num),
		acceptor(servicepool.get_Ioservice()),
		signals(servicepool.get_Ioservice()),
		timeout(DEFAULT_TIMEOUT_INTERVAL),
		timeoutCheckInterval(DEFAULT_TIMEOUTCHECK_INTERVAL),
		bCheckTimeOut(false),
		net_thread_num(net_thread_num)
	{

	}

	service_pool																	servicepool;
	asio::ip::tcp::acceptor													acceptor;
	asio::signal_set																signals;
	asio::error_code															errorCode;
	//¼àÌýµØÖ·
	std::string																		listenAddress;
	//¼àÌý¶Ë¿Ú
	std::string																		listenPort;
	//³¬Ê±¼ì²âÏß³Ì
	loop_thread																	timeOutThread;
	//³¬Ê±(ºÁÃë)
	uint32_t																		timeout;
	//³¬Ê±¼ì²â¼ä¸ô(ºÁÃë)
	uint32_t																		timeoutCheckInterval;
	//ÊÇ·ñ¿ªÆô³¬Ê±¼ì²â
	bool																				bCheckTimeOut;
	//listen actorID
	module_id																	moduleid;

	uint8_t																			net_thread_num;
};

tcp_frame::tcp_frame(const net_message_hander& handler,uint8_t net_thread_num)
	:_hander(handler),_Imp(std::make_shared<Imp>(net_thread_num))
{

}

tcp_frame::~tcp_frame()
{

}

void tcp_frame::listen(const std::string& listenAddress, const std::string& listenPort, module_id moduleid)
{
	_Imp->listenAddress = listenAddress;
	_Imp->listenPort = listenPort;
	_Imp->moduleid = moduleid;

	// Register to handle the signals that indicate when the AsioNetFrame should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.
//	_Imp->signals.add(SIGINT);
//	_Imp->signals.add(SIGTERM);
//#if defined(SIGQUIT)
//	_Imp->signals.add(SIGQUIT);
//#endif // defined(SIGQUIT)
//	_Imp->signals.async_wait(std::bind(&tcp_frame::stop, this));

	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	asio::ip::tcp::resolver resolver(_Imp->servicepool.get_Ioservice());
	asio::ip::tcp::resolver::query query(_Imp->listenAddress, _Imp->listenPort);
	asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	asio::error_code e;

	do
	{
		_Imp->acceptor.open(endpoint.protocol(), e);
		BREAK_IF(e);
		_Imp->acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true), e);
		BREAK_IF(e);
		_Imp->acceptor.bind(endpoint, e);
		BREAK_IF(e);
		_Imp->acceptor.listen();
	} while (0);
	_Imp->errorCode = e;
}

void tcp_frame::postAccept()
{
	auto& ser = _Imp->servicepool.get_service();
	auto sock = std::make_shared<socket>(_hander,ser);
	sock->setmodule_id(_Imp->moduleid);

	_Imp->acceptor.async_accept(sock->get_socket(), [sock, this, &ser](const asio::error_code& e) {
		if (!e)
		{
			ser.add_socket(sock);
			postAccept();
			return;
		}
		_Imp->errorCode = e;
		stop();
	});
}

void tcp_frame::connect(const std::string & ip, const std::string & port, module_id moduleid)
{
	asio::ip::tcp::resolver resolver(_Imp->servicepool.get_Ioservice());
	asio::ip::tcp::resolver::query query(ip, port);
	asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, _Imp->errorCode);
	if (_Imp->errorCode)
	{
		console()->info("resolve endpoint failed:{0} . address:{1}  port{2}.", _Imp->errorCode.message().c_str(), ip.c_str(),port.c_str());
		return;
	}

	auto& ser = _Imp->servicepool.get_service();
	auto sock = std::make_shared<socket>(_hander, ser);
	sock->setmodule_id(moduleid);

	asio::async_connect(sock->get_socket(), endpoint_iterator,
		[this, sock, &ser, ip, port](asio::error_code ec, asio::ip::tcp::resolver::iterator)
	{
		if (!ec)
		{
			ser.add_socket(sock);
		}
		else
		{
			console()->info("connect failed:{0} . address:{1}  port{2}.", ec.message().c_str(), ip.c_str(), port.c_str());
		}
	});
}

void tcp_frame::send(socket_id sockid, const buffer_ptr& data)
{
	 _Imp->servicepool.send(sockid, data);
}

void moon::tcp_frame::close_socket(socket_id sockid, ESocketState state)
{
	_Imp->servicepool.close_socket(sockid, state);
}

void tcp_frame::run()
{
	if (_Imp->errorCode)
	{
		console()->info("tcp_frame start failed:{0}", _Imp->errorCode.message().c_str());
		return;
	}

	console()->info("tcp_frame start succeed");

	_Imp->servicepool.run();

	if (_Imp->bCheckTimeOut)
	{
		console()->info("tcp_frame start check timeout thread succeed");

		_Imp->timeOutThread.set_interval(_Imp->timeoutCheckInterval);
		_Imp->timeOutThread.onUpdate = std::bind(&tcp_frame::checkTimeOut, this, std::placeholders::_1);
		_Imp->timeOutThread.run();
	}

	postAccept();
}

void tcp_frame::stop()
{
	if (_Imp->bCheckTimeOut)
	{
		_Imp->timeOutThread.stop();
	}
	_Imp->servicepool.stop();
	console()->info("tcp_frame stop");
}

int tcp_frame::getErrorCode()
{
	return _Imp->errorCode.value();
}

std::string tcp_frame::getErrorMessage()
{
	return _Imp->errorCode.message();
}

void tcp_frame::setTimeout(uint32_t timeout, uint32_t checkInterval)
{
	_Imp->bCheckTimeOut = true;
	_Imp->timeout = timeout;
	_Imp->timeoutCheckInterval = checkInterval;
}

void tcp_frame::checkTimeOut(uint32_t interval)
{
	auto& servs = _Imp->servicepool.GetAllService();
	for (auto iter = servs.begin(); iter != servs.end(); iter++)
	{
		iter->second->post_check_timeout(_Imp->timeout);
	}
}

MOON_END



