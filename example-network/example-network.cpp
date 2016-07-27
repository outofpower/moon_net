// example-network.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "EchoClient.hpp"
#include "EchoServer.hpp"
#include <common/timer/timer_pool.h>
#include <common/object_pool.hpp>


int main()
{
	while (1)
	{
		auto st = timer_pool::millseconds();
		for (int i = 0; i < 1000; i++)
		{
			//auto pa = std::make_shared<AAA>();
			//auto p = ob.create();

			message msg(64);
		}
		auto et = timer_pool::millseconds();

		printf("%lld ms", et - st);
	}



	//EchoServer svr;
	//svr.Start();

	//EchoClient cl;
	//cl.Start();

    return 0;
}

