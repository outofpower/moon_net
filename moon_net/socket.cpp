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

#include "socket.h"
#include "service.h"
#include "message.h"
#include "moon_net_log.h"

namespace moon
{
	socket::socket(net_message_hander& handler,service& ser)
		:_service(ser),
		_handler(handler),
		_socket(ser.get_ioservice()),
		_isSending(false),
		_sendBuf(IO_BUFFER_SIZE, 0),
		_state(ESocketState::Created),
		_lastRecvTime(0)
	{
		NET_LOG.console("create socket");
	}

	socket::~socket(void)
	{
		NET_LOG.console("release socket[{0:d}] state[{1:d}]", _sockid.value, (int)_state);
	}

	bool socket::start()
	{
		_lastRecvTime			= std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		_remoteEndPoint		= _socket.remote_endpoint(_errorCode);

		_state = ESocketState::Ok;

		if (checkState())
		{
			auto buf = buffer::create(0);
			(*buf) << get_remoteaddress();
			
			message msg(buf);
			msg.set_type(EMessageType::SocketConnect);
			msg.set_sender(_sockid);
			msg.set_receiver(_module_id);
			_handler(msg);
			postRead();
			return true;
		}
		return false;
	}

	void socket::close(ESocketState  state)
	{
		if (_socket.is_open())
		{
			NET_LOG.console("socket address[{0}] forced closed, state[{1:d}]", get_remoteaddress().c_str(), (int)state);
			_state = state;
			//所有异步处理将会立刻调用，并触发 asio::error::operation_aborted
			_socket.shutdown(asio::ip::tcp::socket::shutdown_both, _errorCode);
			if (_errorCode)
			{
				NET_LOG.console("socket address[{0}] shutdown falied:{2}.", get_remoteaddress().c_str(),_errorCode.message());
			}
			_socket.close(_errorCode);
			if (_errorCode)
			{
				NET_LOG.console("socket address[{0}] close falied:{2}.", get_remoteaddress().c_str(), _errorCode.message());
			}
			NET_LOG.console("socket address[{0}] close success.", get_remoteaddress().c_str());
		}
	}

	void socket::postRead()
	{
		if (!checkState())
		{
			return;
		}

		_socket.async_read_some(asio::buffer(_recvBuffer),
			std::bind(&socket::handleRead, shared_from_this(),
				std::placeholders::_1,
				std::placeholders::_2));
	}

	void socket::handleRead(const asio::error_code& e, std::size_t bytes_transferred)
	{
		if (bytes_transferred == 0 || e)
		{
			_state =  ESocketState::ClientClose;
			_errorCode = e;
			onClose();
			return;
		}

		if (!e && bytes_transferred)
		{
			_lastRecvTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			_recvBuf.append(_recvBuffer, bytes_transferred);
	
			while (_recvBuf.size() > sizeof(msg_size_t))
			{
				msg_size_t size = *(msg_size_t*)_recvBuf.data();
				if (size > MAX_MSG_SIZE)
				{
					onClose();
					return;
				}				
				assert(size <= MAX_MSG_SIZE);
				//消息不完整 继续接收消息
				if (_recvBuf.size() < sizeof(msg_size_t) + size)
				{
					break;
				}
				_recvBuf.skip(sizeof(msg_size_t));

				auto buf = buffer::create(size, 24);
				buf->append(_recvBuf.data(), size);
			
				message msg(buf);
				msg.set_type(EMessageType::SocketData);
				msg.set_sender(_sockid);
				msg.set_receiver(_module_id);
				_handler(msg);

				_recvBuf.skip(size);
			}

			postRead();
		}
	}

	void socket::postSend()
	{
		if (!checkState())
			return;

		if (_sendQueue.size() == 0)
			return;

		_sendBuf.clear();

		while ((_sendQueue.size() > 0) && (_sendBuf.size() + _sendQueue.front()->size() < IO_BUFFER_SIZE))
		{
			auto& buf = _sendQueue.front();
			msg_size_t size = 0;
			size += buf->size() &0xFFFF;
			_sendBuf.append((uint8_t*)&size, sizeof(msg_size_t));
			_sendBuf.append(buf->data(), buf->size());

			_sendQueue.pop_front();
		}

		if (0 == _sendBuf.size())
		{
			NET_LOG.console("Temp to send to [%s]  0 bytes message.", get_remoteaddress().c_str());
			return;
		}

		_isSending = true;

		asio::async_write(
			_socket,
			asio::buffer(_sendBuf.data(), _sendBuf.size()),
			std::bind(&socket::handleSend, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	}

	void socket::handleSend(const asio::error_code& e, std::size_t bytes_transferred)
	{
		_isSending = false;
		if (!e)
		{
			postSend();
			return;
		}
		_errorCode = e;
		onClose();
	}

	void socket::onClose()
	{

		auto buf = buffer::create(0);
		(*buf) << (get_remoteaddress());

		message msg(buf);
		msg.set_type(EMessageType::SocketClose);
		msg.set_sender(_sockid);
		msg.set_receiver(_module_id);
		_handler(msg);
		_service.remove_socket(_sockid);
	}

	bool socket::checkState()
	{
		if (_errorCode || _state != ESocketState::Ok)
		{
			return false;
		}
		return true;
	}

	void socket::send(const buffer_ptr& pkt)
	{
		_sendQueue.push_back(pkt);
		if (!_isSending)
		{
			postSend();
		}
	}

	std::string socket::get_remoteaddress()
	{
		asio::error_code err;
		std::string		 straddress;
		do
		{
			auto ep = _socket.remote_endpoint(err);
			BREAK_IF(err);
			auto adr = ep.address();
			auto port = ep.port();
			straddress = adr.to_string(err) + ":";
			straddress += std::to_string(port);
			BREAK_IF(err);
		} while (0);
		return straddress;
	}
};
