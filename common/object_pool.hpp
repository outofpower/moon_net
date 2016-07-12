#pragma once
#include <memory>
#include <stack>
#include <functional>

class SingleThreadLock
{
public:
	void lock()
	{

	}

	void unlock()
	{

	}
};

template<class T>
class GuradLock
{
public:
	GuradLock(T& t)
		:_t(t)
	{
		_t.lock();
	}

	~GuradLock()
	{
		_t.unlock();
	}

private:
	T& _t;
};

template<class T,size_t pool_size = 0, typename Lock = SingleThreadLock>
class object_pool
{
public:
	object_pool() 
	{

	};

	~object_pool()
	{
		GuradLock<Lock> lock(m_Objectlock);
		while (!m_objects.empty())
		{
			auto ptr = m_objects.top();
			delete ptr;
			m_objects.pop();
		}
	};

	object_pool(const object_pool& t) = delete;
	object_pool& operator=(const object_pool& t) = delete;

	template<class... Args>
	std::shared_ptr<T> create(Args... args)
	{
		std::shared_ptr<T> ret = Find();
		if (nullptr == ret)
		{
			ret = std::shared_ptr<T>(new T(args...), std::bind(&object_pool::Release, this, std::placeholders::_1));
		}

		if (nullptr != ret)
		{
			ret->init(args...);
		}
		return ret;
	}

	size_t size()
	{
		return m_objects.size();
	}
private:
	std::shared_ptr<T> Find()
	{
		T* t = nullptr;
		{
			GuradLock<Lock> lock(m_Objectlock);
			if (!m_objects.empty())
			{
				t = m_objects.top();
				m_objects.pop();
			}
		}

		if (t != nullptr)
		{
			return  std::shared_ptr<T>(t, std::bind(&object_pool::Release, this, std::placeholders::_1));
		}
		return nullptr;
	}

	void Release(T* t)
	{
		if (m_objects.size() >= pool_size)
		{
			delete t;
			return;
		}
		GuradLock<Lock> lock(m_Objectlock);
		m_objects.push(t);
	}

private:
	Lock					m_Objectlock;
	std::stack<T*>	m_objects;
};