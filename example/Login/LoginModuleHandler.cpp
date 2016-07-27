#include "LoginModuleHandler.h"
#include "LoginModule.h"

LoginModuleHandler::LoginModuleHandler()
{
}

LoginModuleHandler::~LoginModuleHandler()
{
}

bool LoginModuleHandler::Init()
{
	thisModule().RegisterMessage(EMsgID::MSG_S2S_SERVER_START, this, &LoginModuleHandler::OnServerStart);
	thisModule().RegisterMessage(EMsgID::MSG_S2S_MODULE_START, this, &LoginModuleHandler::OnModuleStart);
	return true;
}

void LoginModuleHandler::OnServerStart(const user_id& userid, uint16_t msgID, binary_reader& data,uint64_t echoid)
{
	Log.console("login module: server start");
	//������ģ��㲥�Լ�
	auto sendmsg = SerializeUtil::serialize(EMsgID::MSG_S2S_MODULE_START);
	binary_writer bw(sendmsg);
	bw << thisModule().GetName();
	bw << thisModule().GetID().value;
	thisModule().Broadcast(sendmsg);
}

void LoginModuleHandler::OnModuleStart(const user_id& userid, uint16_t msgID, binary_reader& data,uint64_t echoid)
{
	std::string name;
	module_id	 id;

	data >> name;
	data >> id.value;

	if (id != thisModule().GetID())
	{
		Log.console("login module: [{0}({1})] module start", name.c_str(), id.value);
		thisModule().AddOtherModule(name,module_id::create(id));
	}
}
