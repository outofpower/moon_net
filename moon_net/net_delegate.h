#pragma once

namespace moon
{
	DECLARE_SHARED_PTR(message);
	DECLARE_SHARED_PTR(socket_connect);

	class net_delegate
	{
	public:
		net_delegate() {}

		virtual ~net_delegate(){}

	protected:
		friend class socket;
		//socket 连接 断开 收到消息
		virtual void socket_message(const message_ptr& msg) = 0;
	};
}
