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
#include "platform_config.hpp"

#include <iostream>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <string>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <functional>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <queue>
#include <array>

#include <memory>

#ifndef INLINE
#define INLINE		inline
#endif

#define BREAK_IF(x) if(x) break;
#define SAFE_DELETE(x) if(nullptr != x) {delete x; x = nullptr;}
#define SAFE_DELETE_ARRAY(x) if(nullptr != x) {delete []x; x = nullptr;}

#define MOON_BEGIN namespace moon{
#define MOON_END   }


#if TARGET_PLATFORM == PLATFORM_WIN32
#include <WinSock2.h>
#define MOON_DLL	__declspec(dllexport)
#define strnicmp	_strnicmp
#ifdef  _WIN64
typedef __int64    ssize_t;
#else
typedef _W64 int   ssize_t;
#endif
#endif

#undef min
#undef max

using gurad_lock = std::unique_lock<std::mutex>;

#define thread_sleep(x)  std::this_thread::sleep_for(std::chrono::milliseconds(x));


template <class T>
bool bool_cast(const T& t)
{
	return (t!=0);
}

#define ADD_READWRITE(varType,varName,funName)\
protected: varType varName =varType();\
public: const varType& get##funName(void) const { return varName; }\
	void set##funName(const varType& v) { varName = v; }


#define ADD_READONLY(varType,varName,funName)\
protected: varType varName =varType();\
public: const varType& get##funName(void) const { return varName; }\
protected:void set##funName(const varType& v) { varName = v; }


#define DECLARE_SHARED_PTR(classname)\
class classname;\
using classname##_ptr = std::shared_ptr<classname>;

#define DECLARE_WEAK_PTR(classname)\
class classname;\
using classname##_wptr std::weak_ptr<classname>;


#define FUNC_BIND0(Func,Target)\
	std::bind(&Func,Target)

#define FUNC_BIND1(Func,Target)\
	std::bind(&Func,Target,std::placeholders::_1)

#define FUNC_BIND2(Func,Target)\
	std::bind(&Func,Target,std::placeholders::_1,std::placeholders::_2)

#define FUNC_BIND3(Func,Target)\
	std::bind(&Func,Target,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)


template<typename T,std::size_t N>
constexpr std::size_t array_szie(T(&)[N])
{
	return N;
}






