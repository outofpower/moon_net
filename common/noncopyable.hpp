#pragma once

namespace moon
{
	class noncopyable
	{
	public:
		noncopyable() {}
		~noncopyable() {}
	private:  // emphasize the following members are private
		noncopyable( const noncopyable& );
		noncopyable& operator=( const noncopyable& );
	};
}


