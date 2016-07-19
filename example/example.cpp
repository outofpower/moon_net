// example.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <actor_manager.h>
#include "ServerDefine.h"
#include "Config/ModuleConfig.h"
#include "./gate/GateModule.h"
#include "./login/LoginModule.h"

void createactor(actor_manager& actmgr, std::unordered_map<std::string, std::string>& config)
{
	if (config.find("type") == config.end())
		return;

	std::string type = config["type"];

	if (type == "GateModule")
	{
		actmgr.create_actor<GateModule>(config);
	}
	else if (type == "LoginModule")
	{
		actmgr.create_actor<LoginModule>(config);
	}
	else if (type == "WorldModule")
	{
		
	}
	else if (type == "SceneModule")
	{
		
	}

}


#if TARGET_PLATFORM == PLATFORM_WIN32

bool g_exit = false;

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_C_EVENT)
	{
		g_exit = true;
	}
	else if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		g_exit = true;
	}
	return TRUE;
}

#endif

int main()
{
	{
		ServerLog log;
		log.Init(".\\");

		server_config svr;
		svr.load("server_config.xml");

		actor_manager actmgr;
		actmgr.init(1);

		for (auto& c : svr.configs)
		{
			createactor(actmgr, c.config);
		}

		log.trace("server run");

		actmgr.run();

		{
			auto sdata = SerializeUtil::serialize(EMsgID::MSG_S2S_SERVER_START);
			message msg(sdata);
			msg.set_type(EMessageType::ActorData);
			actmgr.broadcast_message(module_id::create(0), msg);
		}

#if TARGET_PLATFORM == PLATFORM_WIN32

		SetConsoleCtrlHandler(HandlerRoutine, TRUE);
#endif

		while (!g_exit)
		{
			thread_sleep(1000);
		}

		actmgr.stop();

		log.trace("server stop");
	}


	return 0;
}


