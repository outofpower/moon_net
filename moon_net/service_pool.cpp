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

#include  "service_pool.h"

using namespace moon;

service_pool::service_pool(uint8_t pool_size)
	:_nextservice(0)
{
	assert(pool_size >0);
	for(uint8_t i = 0; i<pool_size ; ++i)
	{
		auto tmp = service::create();
		tmp->set_id(i);
		_services[i] = tmp;
	}
}


service_pool::~service_pool(void)
{
}

void service_pool::run()
{
	_threads.clear();
	// Create a pool of threads to run all of the io_services.
	for (uint32_t  i = 0; i < _services.size(); ++i)
	{
		std::shared_ptr<std::thread> thread(new std::thread(
			std::bind(&service::run, _services[i])));
		_threads.push_back(thread);
	}
}

void service_pool::stop()
{
	// Explicitly stop all io_services. 
	for (uint32_t i = 0; i < _services.size(); ++i)
		_services[i]->stop();

	for (uint32_t i = 0; i < _threads.size(); ++i)
	{
		if(_threads[i]->joinable())
		_threads[i]->join();
	}
}

void moon::service_pool::send(socket_id sockid, const buffer_ptr& data)
{
	uint8_t servicesid = (sockid.value >> 24)&0xFF;
	auto iter = _services.find(servicesid);
	if (iter != _services.end())
	{
		iter->second->send(sockid, data);
	}
}

void moon::service_pool::close_socket(socket_id sockid, ESocketState state)
{
	uint8_t servicesid = (sockid.value >> 24) & 0xFF;
	auto iter = _services.find(servicesid);
	if (iter != _services.end())
	{
		iter->second->close_socket(sockid, state);
	}
}

service& service_pool::get_service()
{
	// Use a round-robin scheme to choose the next io_service to use. 
	service_ptr& service = _services[_nextservice];
	++_nextservice;
	if (_nextservice == _services.size())
		_nextservice = 0;
	return *service;
}

asio::io_service & service_pool::get_Ioservice()
{
	return get_service().get_ioservice();
}

