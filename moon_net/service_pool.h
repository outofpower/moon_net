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
#include "service.h"

MOON_BEGIN

class service_pool 
{
public:
	service_pool(uint8_t pool_size);
	~service_pool(void);

	void run();

	void stop();

	void	send(socket_id sockid, const buffer_ptr& data);

	void	close_socket(socket_id sockid, ESocketState state);

	service& get_service();
	asio::io_service& get_Ioservice();
	std::unordered_map<uint8_t, service_ptr>& GetAllService() { return _services; }

private:
	std::unordered_map<uint8_t, service_ptr>	_services;
	// The next service to use for a connection.
	uint8_t															_nextservice;
	std::vector<std::shared_ptr<std::thread>>	_threads;
};

MOON_END



