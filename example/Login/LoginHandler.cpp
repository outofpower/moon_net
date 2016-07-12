#include "LoginHandler.h"
#include "LoginModule.h"
#include "AccountDataCache.h"

#include <MsgLogin.pb.h>
using namespace NetMessage;

LoginHandler::LoginHandler(ModuleBases& module)
	:ModuleBehaviour(module),m_Accounts(nullptr)
{

}

LoginHandler::~LoginHandler(void)
{

}

bool LoginHandler::Init()
{
	thisModule().RegisterMessage(EMsgID::MSG_C2S_REQ_LOGIN, this, &LoginHandler::OnRequestLogin);

	m_Accounts = thisModule().GetModuleBehaviour<AccountDataCache>();
	assert(m_Accounts != nullptr);

	return true;
}

void LoginHandler::OnClientClose(account_id accountID, player_id playerID)
{
	if (m_Accounts->OffLine(accountID))
	{
		Log.trace("login module:client close [{0}] offline success", accountID.value);
	}
	else
	{
		Log.trace("login module:client close [{0}] offline failed", accountID.value);
	}
}

void LoginHandler::OnRequestLogin(const user_id& userid, uint16_t msgID, buffer_reader& data,uint64_t echoid)
{
	uint64_t   login_serial_num = 0;
	account_id accountID;
	uint32_t  socketid = 0;
	std::string password;

	data >> login_serial_num;
	data >> accountID.value;
	data >> password;

	S2CLogin_ERet ret = S2CLogin_ERet_Ok;
	do 
	{
		
		////ÕËºÅ²»´æÔÚ
		//if (0 == this_module().GetAccountDatas().IsExistAccount(accountID))
		//{
		//	ret = S2CLogin_ERet_AccountNotExsit;
		//}
		//ÖØ¸´µÇÂ½
		if (m_Accounts->IsOnline(accountID))
		{
			ret = S2CLogin_ERet_LoginAgain;
			break;
		}
		////ÃÜÂë´íÎó
		//if (!this_module().GetAccountDatas().Match(accountID, password))
		//{
		//	ret = S2CLogin_ERet_PasswordError;
		//}
		//ÕËºÅÒÑ¾­ÔÚÏß
		if (m_Accounts->IsOnline(accountID))
		{
			ret = S2CLogin_ERet_LoginAgain;
			break;
		}

		m_Accounts->AddAccount(accountID, "", password);

		if (m_Accounts->OnLine(accountID))
		{
			Log.trace("login module:account login success [{0}] ", accountID.value);
		}
		else
		{
			assert(0);
		}

	} while (0);

	auto sendMsg = buffer::create(100);
	(*sendMsg) << uint32_t(ret);
	(*sendMsg) << login_serial_num;
	(*sendMsg) << accountID.value;
	thisModule().Send(thisModule().getGateModule(),sendMsg, EMessageType::ActorData, echoid);
}



