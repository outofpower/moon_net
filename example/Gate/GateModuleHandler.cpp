#include "GateModuleHandler.h"
#include "GateModule.h"

GateModuleHandler::GateModuleHandler()
{
}

GateModuleHandler::~GateModuleHandler()
{
}

bool GateModuleHandler::Init()
{
	thisModule().RegisterMessage(EMsgID::MSG_S2S_SERVER_START, this, &GateModuleHandler::OnServerStart);
	thisModule().RegisterMessage(EMsgID::MSG_S2S_MODULE_START, this, &GateModuleHandler::OnModuleStart);
	return true;
}

void GateModuleHandler::OnServerStart(const user_id& userid,uint16_t msgID, buffer_reader& data,uint64_t echoid)
{
	Log.console("gate module: server start");
	//向其他模块广播自己
	auto sendmsg = SerializeUtil::serialize(EMsgID::MSG_S2S_MODULE_START);
	
	(*sendmsg) << thisModule().GetName();
	(*sendmsg) << thisModule().GetID().value;
	thisModule().Broadcast(sendmsg);
}

void GateModuleHandler::OnModuleStart(const user_id& userid,uint16_t msgID, buffer_reader& data,uint64_t echoid)
{
	std::string name;
	module_id	 id;

	data >> name;
	data >> id.value;

	if (id != thisModule().GetID())
	{
		Log.console("gate module: [{0}({1})] module start", name.c_str(), id.value);
		thisModule().AddOtherModule(name, id);

		if (name == "login")
		{
			//thisModule().set_login_module(id);
		}
		else if (name == "world")
		{
			//this_module().set_world_module(id);
		}
	}
}
