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

class EchoServer :noncopyable
{
public:
	EchoServer()
	{

	}

	~EchoServer()
	{
		if (m_Net != nullptr)
		{
			//关闭网络线程
			m_Net->stop();
		}
	}

	void Start()
	{
		auto handler = std::bind(&EchoServer::OnNetMessage, this, std::placeholders::_1);
		m_Net = std::make_shared<tcp_frame>(handler,1);//2条网络线程

		//监听本机的 11111 端口，由于此处只是用网络库，不关心模块，模块ID为0 即可
		m_Net->listen("127.0.0.1", "11111", module_id::create(0));

		//启动网络线程
		m_Net->run();

		//正常情况下 应该阻塞主线程，防止主线程退出。由于稍后客户端会运行在主线程内，不再阻塞主线程
	}

private:
	void OnNetMessage(const message& msg)
	{
		switch (msg.get_type())
		{
		case EMessageType::SocketConnect:
		{
			//连接，消息内容默认为远程主机地址
			binary_reader br(msg.data(), msg.size());
			std::string addr = br.read<std::string>();
			std::cout <<"SERVER:client connect "<<addr<< std::endl;
			break;
		}
		case EMessageType::SocketData:
		{
			//收到数据，message 保存有发送者和接收者的ID.
			//如果是网络消息，那么发送者为网络连接ID

			//获取网络连接ID
			auto sockID = msg.get_socket_id();


			binary_reader br(msg.data(), msg.size());
			//获取发来的信息
			std::string  clientMsg  = br.read<std::string>();

			////把信息发送回去
			//message smsg(clientMsg.size() + 1);

			//binary_writer bw(smsg);
			//bw << clientMsg;

			//m_Net->send(sockID, smsg);


			std::cout << "SERVER:echo msg " << clientMsg << std::endl;
			break;
		}
		case EMessageType::SocketClose:
		{
			//断开
			binary_reader br(msg.data(), msg.size());
			std::string addr;
			br >> addr;
			std::cout << "SERVER:client close " << addr << std::endl;
			break;
		}
		default:
			break;
		}
	}

private:
	std::shared_ptr<tcp_frame>   m_Net;
};

