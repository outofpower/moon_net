// example-network.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "EchoClient.hpp"
#include "EchoServer.hpp"

int main()
{
	EchoServer svr;
	svr.Start();

	EchoClient cl;
	cl.Start();

    return 0;
}

