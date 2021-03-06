#pragma once
#include <common/noncopyable.hpp>
#include <common/memory_stream.hpp>
#include <common/binary_writer.hpp>
#include <common/binary_reader.hpp>
#include <tcp_frame.h>
#include <message.h>
#include <string>
#include <iostream>
#include <functional>

using namespace moon;

class EchoClient
{
public:

	void Start()
	{
		auto handler = std::bind(&EchoClient::OnNetMessage, this, std::placeholders::_1);
		m_Net = std::make_shared<tcp_frame>(handler, 1);//1条网络线程

		//连接本机的 11111 端口，由于此处只是用网络库，不关心模块，模块ID为0 即可
		auto id = m_Net->sync_connect("127.0.0.1", "11111", module_id::create(0));

		if (id.value == 0)
		{
			std::cout << "CLIENT : connect server failed" << std::endl;
			return;
		}

		//启动网络线程
		m_Net->run();

		std::string str;

		std::cin >> str;

		while (str != "exit")
		{
			{
				std::cout << "CLIENT:send msg " << str << std::endl;

				message msg(str.size() + 1);
				binary_writer bw(msg);
				bw << str;

				m_Net->send(id, msg);
			}
	
			std::cin >> str;
		}

		m_Net->stop();
	}

private:
	void OnNetMessage(const message& msg)
	{
		switch (msg.get_type())
		{
		case EMessageType::SocketConnect:
		{
			binary_reader br(msg.data(), msg.size());
			std::string addr = br.read<std::string>();
			std::cout << "CLIENT:connect server:" << addr << std::endl;
			break;
		}
		case EMessageType::SocketData:
		{
			//获取网络连接ID
			auto sockID = msg.get_socket_id();
			binary_reader br(msg.data(), msg.size());
			//获取发来的信息
			std::string  clientMsg = br.read<std::string>();
			std::cout << "CLIENT:recv msg " << clientMsg << std::endl;
			break;
			break;
		}
		case EMessageType::SocketClose:
		{
			//断开，消息内容默认为客户端地址
			binary_reader br(msg.data(), msg.size());
			std::string addr = br.read<std::string>();
			std::cout << "CLIENT:server close " << addr << std::endl;
			break;
		}
		default:
			break;
		}
	}

private:
	std::shared_ptr<tcp_frame>   m_Net;
};