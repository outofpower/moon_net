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
#include <cstdint>

template<class  _Type, class _Child>
struct base_id
{
	using Type = _Type;
	Type  value;
	base_id()
		:value(0)
	{

	}

	base_id(const base_id& other)
	{
		value = other.value;
	}

	operator Type() const
	{
		return value;
	}

	operator Type()
	{
		return value;
	}

	_Child& operator =(const _Child& rt)
	{
		value = rt.value;
		return *(static_cast<_Child*>(this));
	}

	bool operator ==(const _Child& rt) const
	{
		return (value == rt.value);
	}

	bool operator ==(const _Child& rt)
	{
		return (value == rt.value);
	}

	bool operator !=(const _Child& rt) const
	{
		return (value != rt.value);
	}

	bool operator !=(const _Child& rt)
	{
		return (value != rt.value);
	}

	static _Child create(Type v)
	{
		_Child tmp;
		tmp.value = v;
		return tmp;
	}
};
