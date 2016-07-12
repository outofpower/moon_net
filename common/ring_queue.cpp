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

#include "ring_queue.h"
#include <iostream>
#include <algorithm>

MOON_BEGIN

RingQueue::RingQueue()
	:_in(0),_out(0),_size(0)
{
	_capacity   = 8192 * 8192;
	_buffer		= new uint8_t[_capacity];
}

RingQueue::~RingQueue()
{
	delete []_buffer;
}



void RingQueue::Write(const uint8_t* pbuffer, uint32_t len)
{
	checkWrite(len);
	len = std::min(len,(_capacity-_size));
	/* first put the data starting from fifo->in to buffer end */
	uint32_t n = std::min(len,_capacity - _in);
	memcpy(_buffer + _in, pbuffer,n);
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(_buffer, pbuffer + n, len - n);
	_in += len;
	_in %=_capacity;
	_size += len;
}

bool RingQueue::Read(uint32_t len, std::vector<uint8_t>& data)
{
	if(_size < len)
		return false;

	len = std::min(len,_size.load());

	data.resize(len);

	/* first get the data from fifo->out until the end of the buffer */
	uint32_t n = std::min(len, _capacity - _out);

	memcpy(&data[0], _buffer + _out, n);
	memcpy(&data[0] + n , _buffer, len - n);

	_out += len;
	_out %= _capacity;
	_size -= len;

	_notFull.notify_one();
	return true;
}

bool RingQueue::Copy(uint8_t* pbuffer, uint32_t len)
{
	if(_size < len)
		return false;

	len = std::min(len,_size.load());

	/* first get the data from fifo->out until the end of the buffer */
	uint32_t n = std::min(len, _capacity - _out);

	memcpy(pbuffer, _buffer + _out, n);
	memcpy(pbuffer + n , _buffer, len - n);

	return true;
}

bool RingQueue::Ship(uint32_t len)
{
	if(_size < len)
		return false;
	len = std::min(len,_size.load());
	_out += len;
	_out %= _capacity;
	_size -= len;
	_notFull.notify_one();
	return true;
}

MOON_END
