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
#include <functional>
#include <vector>

namespace moon
{
	template<typename T>
	class delegate;

	template<class... Args>
	class delegate<void(Args...)>
	{
	public:
		typedef std::function<void(Args...)> Func;

		template<class... _Types>
		void operator()(_Types&&... args)
		{
			for (auto& f : _Funcs)
			{
				f(std::forward<_Types>(args)...);
			}
		}

		delegate()
		{

		}

		delegate(const Func& func)
		{
			_Funcs.push_back(func);
		}

		delegate(Func&& func)
		{
			_Funcs.emplace_back(std::forward<Func>(func));
		}

		delegate(const delegate& dele)
		{
			_Funcs = dele._Funcs;
		}

		delegate(delegate&& dele)
		{
			_Funcs = std::move(dele._Funcs);
		}

		delegate& operator=(const delegate& dele)
		{
			_Funcs = dele._Funcs;
			return *this;
		}

		delegate& operator=(const Func& func)
		{
			_Funcs.clear();
			_Funcs.push_back(func);
			return *this;
		}

		delegate& operator=(Func&& func)
		{
			_Funcs.clear();
			_Funcs.emplace_back(std::forward<Func>(func));
			return *this;
		}

		delegate& operator+=(const Func& func)
		{
			_Funcs.push_back(func);
			return *this;
		}

		delegate& operator+=(Func&& func)
		{
			_Funcs.emplace_back(std::forward<Func>(func));
			return *this;
		}
	private:
		std::vector<Func>  _Funcs;
	};

}

