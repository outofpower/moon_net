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
#include <unordered_map>
#include <functional>

template<class Key, class Value>
class hash_map :public std::unordered_map<Key, Value>
{
public:
	bool contains_key(const Key& key)
	{
		auto iter = find(key);
		return (iter != end());
	}

	bool contains_key(const Key& key) const
	{
		auto iter = find(key);
		return (iter != end());
	}

	template<typename T, typename Func>
	bool contains_value(const T& v,
		const Func& f)
	{
		auto iter = find_by_value(v, f);
		return (iter != end());
	}

	template<typename T, typename Func>
	bool contains_value(const T& v,
		const Func& f) const
	{
		auto iter = find_by_value(v, f);
		return (iter != end());
	}

	template<typename T,typename Func>
	iterator find_by_value(const T& v,
		const Func& f)
	{
		for (auto& _it = begin(); _it != end(); _it++)
		{
			if (f(_it->second, v))
			{
				return _it;
			}
		}
		return end();
	}

	//
	template<typename T, typename Func>
	const_iterator find_by_value(const T& v,
		const Func& f) const
	{
		for (auto& _it  = begin(); _it != end(); _it++)
		{
			if (f(_it->second,v))
			{
				return _it;
			}
		}
		return end();
	}

	template<typename T, typename Func>
	bool erase_by_value(const T& v,const Func& f)
	{
		auto _it = find_by_value(v, f);
		if (_it != end())
		{
			erase(_it);
			return true;
		}
		return false;
	}

	Value* add(const Key& k, Value&& v)
	{
		auto& ret = emplace(k, std::forward<Value>(v));
		if (!ret.second)
		{
			return nullptr;
		}
		return &(ret.first->second);
	}

	Value* add(const Key& k, const Value& v)
	{
		auto& ret =insert(std::make_pair(k,v));
		if (!ret.second)
		{
			return nullptr;
		}
		return &(ret.first->second);
	}

	Value* find_value(const Key& k)
	{
		auto iter = find(k);
		if (iter == end())
		{
			return nullptr;
		}
		return &(iter->second);
	}

	const Value* find_value(const Key& k) const
	{
		auto iter = find(k);
		if (iter == end())
		{
			return nullptr;
		}
		return &(iter->second);
	}
};
