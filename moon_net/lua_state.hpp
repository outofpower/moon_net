#pragma once
#include <cassert>
#include <string>
#include "lua.hpp"
#include <functional>

using cpp_function = std::function<int(lua_State*)>;
using cpp_function_pointer = cpp_function*;

class lua_state
{
public:
	lua_state(lua_State* state = nullptr)
		:_state(state), _release(state == nullptr)
	{
		if (nullptr == _state)
		{
			create_state();
		}
	}

	virtual ~lua_state()
	{
		if(_release)
		release_state();
	}

protected:
	void create_state()
	{
		_state = luaL_newstate();
	}

	void release_state()
	{
		if (nullptr != _state)
		{
			lua_close(_state);
			_state = nullptr;
		}
	}


	static int function_wrapper(lua_State* L)
	{
		lua_state state(L);
		lua_Debug debugInfo = state.get_debugInfo("nSlu");
		int upvalues = debugInfo.nups;
		if (upvalues >= 1)
		{
			int upvalueIndex = state.upvalueIndex(1);
			state.pushvalue(upvalueIndex);
			int t = state.type(-1);
			if (t == LUA_TUSERDATA)
			{
				auto f = (cpp_function_pointer*)state.pop<void*>(upvalueIndex);
				state.pop(1);
				if (f != nullptr)
				{
					return (**f)(L);
				}
				else
				{
					const std::string traceBack = state.traceback();
					state.error("Invalid function pointer!: %s", traceBack.c_str());
					return 1;
				}
			}
			else
			{
				const std::string traceBack = state.traceback();
				state.error("Stack corrupted!: %s", traceBack.c_str());
				return 1;
			}
		}
		else
		{
			const std::string traceBack = state.traceback();
			state.error("Closure upvalues corrupted!: %s", traceBack.c_str());
			return 1;
		}
		return 1;
	}
public:
	//open lua standard lib
	void open_libs()
	{
		luaL_openlibs(_state);
	}

	bool load_file(const std::string & file_name) {
		int ret = luaL_loadfile(_state, file_name.c_str());	
		if (ret != 0)
		{
			//LUA_ERRSYNTAX: 在预编译时碰到语法错误；
			//LUA_ERRMEM: 内存分配错误。
			//LUA_ERRFILE : cannot open/read the file
			return false;
		}
		return true;
	}

	bool run_file(const std::string & file_name)
	{
		if (load_file(file_name))
		{
			int n = lua_pcall(_state, 0, LUA_MULTRET, 0);
			//LUA_ERRRUN: 运行时错误。 
			//LUA_ERRMEM: 内存分配错误。 对于这种错，Lua 调用不了错误处理函数。
			//LUA_ERRERR : 在运行错误处理函数时发生的错误。
			if (n == 0)
			{
				return true;
			}
		}
		return false;
	}

	bool load_string(const std::string & chunk) {
		int ret = luaL_loadstring(_state, chunk.c_str());
		if (ret != 0) {
			//LUA_ERRSYNTAX: 在预编译时碰到语法错误；
			//LUA_ERRMEM: 内存分配错误。
			return true;
		}
		return false;
	}

	//'n': 填充 name 及 namewhat 域；
	//'S': 填充 source， short_src， linedefined， lastlinedefined，以及 what 域；
	//'l': 填充 currentline 域；
	//'u': 填充 nups 域；
	//'L': 压一个 table 入栈，这个 table 中的整数索引用于描述函数中哪些行是有效行
	const lua_Debug get_debugInfo(const std::string & what) {
		lua_Debug debugInfo;
		assert(lua_getstack(_state, 0, &debugInfo) == 1);
		assert(lua_getinfo(_state, what.c_str(), &debugInfo) != 0);
		return debugInfo;
	}

	const lua_Debug get_stack(const int level) {
		lua_Debug debugInfo;
		assert(lua_getstack(_state, level, &debugInfo) == 1);
		return debugInfo;
	}

	void error(const char * fmt, ...) {
		char buffer[1024];
		va_list args;
		va_start(args, fmt);
#if defined(_WIN32) && defined(_MSC_VER)
		vsprintf_s(buffer, fmt, args);
#else
		vsprintf(buffer, fmt, args);
#endif
		luaL_error(_state, "%s", buffer);
		va_end(args);
	}

	const std::string traceback() {
		lua_Debug info;
		int level = 0;
		std::string outputTraceback;
		char buffer[4096];

		while (lua_getstack(_state, level, &info)) {
			lua_getinfo(_state, "nSl", &info);

#if defined(_WIN32) && defined(_MSC_VER)
			sprintf_s(buffer, "  [%d] %s:%d -- %s [%s]\n",
				level, info.short_src, info.currentline,
				(info.name ? info.name : "<unknown>"), info.what);
#else
			sprintf(buffer, "  [%d] %s:%d -- %s [%s]\n",
				level, info.short_src, info.currentline,
				(info.name ? info.name : "<unknown>"), info.what);
#endif
			outputTraceback.append(buffer);
			++level;
		}
		return outputTraceback;
	}

public:
	/*
	Basic stack operations
	*/

	inline int gettop()
	{
		return lua_gettop(_state);
	}

	inline void settop(int index)
	{
		lua_settop(_state, index);
	}

	inline int upvalueIndex(const int index)
	{
		return lua_upvalueindex(index);
	}

	inline void pop(int n = 1)
	{
		lua_pop(_state, n);
	}

	inline void insert(int index)
	{
		lua_replace(_state, index);
	}

	inline void replace(int index)
	{
		lua_replace(_state, index);
	}

	inline void remove(int index)
	{
		lua_remove(_state, index);
	}

	inline void get_global(const std::string & name)
	{
		lua_getglobal(_state, name.c_str());
	}

	inline void set_global(const std::string & name)
	{
		lua_setglobal(_state, name.c_str());
	}

	/*
	Tables
	*/

	inline void newtable()
	{
		lua_newtable(_state);
	}

	//将被预分配 narr 个元素的数组空间 以及 nrec 个元素的非数组空间
	inline void newtable(int narray, int nrec)
	{
		lua_createtable(_state, narray, nrec);
	}

	inline void get_table(const int index = -3)
	{
		lua_gettable(_state, index);
	}

	//把栈顶的值 赋值 给 指定索引处table[key].(栈顶之下的那个值为key)这个函数会把键和值都从堆栈中弹出
	inline void set_table(const int index = -3)
	{
		lua_settable(_state, index);
	}

	inline void concat(const int count)
	{
		lua_concat(_state, count);
	}

	inline void raw_get(const int index = -3)
	{
		lua_rawget(_state, index);
	}

	inline void raw_get(const int n, const int index = -3)
	{
		lua_rawgeti(_state, index, n);
	}

	inline void raw_set(const int index = -3)
	{
		lua_rawset(_state, index);
	}

	inline void raw_set(const int n, const int index = -3)
	{
		lua_rawseti(_state, index, n);
	}

	inline void getfield(const std::string & key, const int index = -2)
	{
		lua_getfield(_state, index, key.c_str());
	}

	inline void getfield(const int key, const int index = -2)
	{
		lua_pushinteger(_state, key);
		lua_gettable(_state, index);
	}

	inline void setfield(const std::string & key, const int index = -2)
	{
		lua_setfield(_state, index, key.c_str());
	}

	inline void pushclosure(lua_CFunction fn, int n)
	{
		lua_pushcclosure(_state, fn, n);
	}

	inline void pushlstring(const char*value, size_t len)
	{
		lua_pushlstring(_state, value, len);
	}

	inline void pushbytes(const unsigned char*value, size_t len)
	{
		lua_pushlstring(_state,(char*)value, len);
	}

	inline void pushliteral(const std::string value)
	{
		lua_pushlstring(_state, value.c_str(), value.size());
	}

	inline void pushNil()
	{
		lua_pushnil(_state);
	}

	inline void pushvalue(const int index)
	{
		lua_pushvalue(_state, index);
	}

	inline void setfieldlstring(const std::string & name, const std::string & value, size_t len, const int index = -1)
	{
		pushlstring(value.c_str(), len);
		lua_setfield(_state, index, name.c_str());
	}

	inline int ref(const int index = LUA_REGISTRYINDEX)
	{
		return luaL_ref(_state, index);
	}

	inline void  unref(const int ref, const int index = LUA_REGISTRYINDEX)
	{
		luaL_unref(_state, index, ref);
	}

	/*
	Values
	*/
	inline const size_t objLen(const int index = -1)
	{
		return lua_objlen(_state, index);
	}

	inline const int type(const int index = -1)
	{
		return lua_type(_state, index);
	}

	template<int TYPE> inline const bool is(const int index = -1)
	{
		return lua_type(_state, index) == TYPE;
	}

	inline const std::string typeName(const int index = -1)
	{
		return std::string(lua_typename(_state, index));
	}

	inline void * newuserdata(size_t size)
	{
		return lua_newuserdata(_state, size);
	}

	inline void * checkuserdata(const int narg, const std::string& name)
	{
		if (lua_type(_state, narg) == LUA_TUSERDATA) {
			return luaL_checkudata(_state, narg, name.c_str());
		}
		else {
			return nullptr;
		}
	}

	inline void * getuserdata(const int narg, const std::string& name)
	{
		int t = lua_type(_state, narg);

		if (t == LUA_TUSERDATA) {
			lua_getmetatable(_state, narg);
			luaL_getmetatable(_state, name.c_str());
			if (lua_equal(_state, -2, -1) == 1) {
				lua_pop(_state, 2);
				return lua_touserdata(_state, narg);
			}
			else {
				lua_pop(_state, 2);
				return nullptr;
			}
		}
		else {
			return nullptr;
		}
	}


	/*
	Metatables
	*/

	inline void getmetatable(const int index = -1)
	{
		lua_getmetatable(_state, index);
	}

	inline void getmetatable(const std::string & name)
	{
		luaL_getmetatable(_state, name.c_str());
	}

	//把一个 table 弹出堆栈，并将其设为给定索引处的值的 metatable 。
	inline void setmetatable(const int index = -2)
	{
		lua_setmetatable(_state, index);
	}

	inline bool newmetatable(const std::string & name)
	{
		return luaL_newmetatable(_state, name.c_str()) == 1;
	}

	inline bool getMetaField(const std::string & name, const int index = -1)
	{
		return luaL_getmetafield(_state, index, name.c_str()) != 0;
	}

	/*
	Functions
	*/
	void call(const int nargs, const int nresults)
	{
		lua_call(_state, nargs, nresults);
	}

	void pcall(const int nargs, const int nresults, const int errFunction = 0)
	{
		int result = lua_pcall(_state, nargs, nresults, errFunction);
		if (result != 0) {
			const std::string errMessage = lua_tostring(_state, -1);
			printf("\r\n%s\r\n", errMessage.c_str());
			//if (result == LUA_ERRRUN) {
			//	throw std::runtime_error("Runtime error: " + errMessage);
			//}
			//else if (result == LUA_ERRMEM) {
			//	throw std::runtime_error("Allocation error: " + errMessage);
			//}
			//else if (result == LUA_ERRERR) {
			//	throw std::runtime_error("Error handler error: " + errMessage);
			//}
			//else {
			//	throw std::runtime_error("Unknown error: " + errMessage);
			//}
		}
	}


	template<typename TRet>
	TRet call(const char* name)
	{
		lua_pushstring(_state, name);
		lua_gettable(_state, LUA_GLOBALSINDEX);
		lua_pcall(_state, 0, 1, 0);
		return pop<TRet>(_state);
	}

	template<typename TRet, typename T1>
	TRet call(const char* name, T1 arg)
	{
		lua_pushstring(_state, name);
		lua_gettable(_state, LUA_GLOBALSINDEX);
		push(_state, arg);
		lua_pcall(_state, 1, 1, 0);
		return pop<TRet>(_state);
	}

	template<typename TRet, typename T1, typename T2>
	TRet call(const char* name, T1 arg1, T2 arg2)
	{
		lua_pushstring(_state, name);
		lua_gettable(_state, LUA_GLOBALSINDEX);
		assert(lua_isfunction(L, -1));
		push(_state, arg1);
		push(_state, arg2);
		lua_pcall(_state, 2, 1, 0);
		return pop<TRet>(_state, -1);
	}


	inline void regValue(const int n)
	{
		lua_rawgeti(_state, LUA_REGISTRYINDEX, n);
	}


	template<typename T>
	void push(T v)
	{
		static_assert(std::is_arithmetic<T>::value, "T must number.");
		lua_pushnumber(_state, v);
	}

	template<>
	void push(int64_t v)
	{
		auto* userdata = static_cast<decltype(v)*>(lua_newuserdata(_state, sizeof(v)));
		(*userdata) = v;
		lua_pushstring(_state, "__s64");
		lua_gettable(_state, LUA_GLOBALSINDEX);
		lua_setmetatable(_state, -2);
	}

	template<>
	void push(uint64_t v)
	{
		auto* userdata = static_cast<decltype(v)*>(lua_newuserdata(_state, sizeof(v)));
		(*userdata) = v;
		lua_pushstring(_state, "__u64");
		lua_gettable(_state, LUA_GLOBALSINDEX);
		lua_setmetatable(_state, -2);
	}

	template<>
	void push(const char* str)
	{
		lua_pushstring(_state, str);
	}

	template<>
	void push(bool v)
	{
		lua_pushboolean(_state, v);
	}

	template<>
	void push(cpp_function value)
	{
		auto f = static_cast<cpp_function_pointer*>(newuserdata(sizeof(cpp_function_pointer)));
		*f = new cpp_function(value);
		pushclosure(function_wrapper, 1);
	}

	template<>
	void push(lua_CFunction value)
	{
		lua_pushcfunction(_state, value);
	}

	inline void push(cpp_function value, int n)
	{
		auto f = static_cast<cpp_function_pointer*>(newuserdata(sizeof(cpp_function_pointer)));
		*f = new cpp_function(value);
		pushclosure(function_wrapper, n + 1);
	}

	template<class T>
	T pop(int index)
	{
		static_assert(std::is_arithmetic<T>::value, "T must number.");
		assert(lua_isnumber(_state, index));
		return static_cast<T>(lua_tonumber(_state, index));
	}

	template<>
	std::string pop(int index)
	{
		assert(lua_isstring(_state, index));
		std::string str = lua_tostring(_state, index);
		lua_pop(_state, 1);
		return str;
	}

	template<>
	int64_t pop(int index)
	{
		int64_t v = 0;
		if (lua_isnumber(_state, index))
		{
			v = static_cast<int64_t>(lua_tonumber(_state, index));
		}
		else
		{
			v = *static_cast<int64_t*>(lua_touserdata(_state, index));
		}
		lua_pop(_state, 1);
		return v;
	}

	template<>
	uint64_t pop(int index)
	{
		uint64_t v = 0;
		if (lua_isnumber(_state, index))
		{
			v = static_cast<uint64_t>(lua_tonumber(_state, index));
		}
		else
		{
			v = *static_cast<uint64_t*>(lua_touserdata(_state, index));
		}

		lua_pop(_state, 1);
		return v;
	}

	template<>
	void* pop(int index)
	{
		return lua_touserdata(_state, index);
	}

	template<class T>
	void setfield(const std::string & name, T value, const int index) {
		push<T>(value);
		lua_setfield(_state, index, name.c_str());
	}

protected:
	bool			 _release;

public:
	lua_State*  _state;
};
