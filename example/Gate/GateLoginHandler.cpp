#include "GateLoginHandler.h"
#include "GateModule.h"
#include "LoginData.h"
#include "Connection.h"
#include "GateModule.h"

#include <MsgLogin.pb.h>
using namespace NetMessage;

GateLoginHandler::GateLoginHandler()
	:m_LoginDatas(nullptr)
{
}

GateLoginHandler::~GateLoginHandler()
{

}

bool GateLoginHandler::Init()
{
	thisModule().RegisterMessage(EMsgID::MSG_C2S_REQ_LOGIN, this, &GateLoginHandler::OnRequestLogin);
	thisModule().RegisterMessage(EMsgID::MSG_S2S_SET_ACCOUNT_PLAYERID, this, &GateLoginHandler::OnSetPlayerID);
	thisModule().RegisterMessage(EMsgID::MSG_S2S_SET_PLAYER_SCENEID, this, &GateLoginHandler::OnSetSceneID);
	
	m_LoginDatas = thisModule().GetModuleBehaviour<LoginModelManager>();
	assert(nullptr != m_LoginDatas);

	m_Connects = thisModule().GetModuleBehaviour<ConnectionManager>();
	assert(nullptr != m_Connects);

	m_GateModule = dynamic_cast<GateModule*>((&thisModule()));
	assert(nullptr != m_GateModule);
	return true;
}

void GateLoginHandler::OnClientClose(account_id accountID, player_id playerID)
{
	ClientExitServer(accountID, playerID);
}

void GateLoginHandler::ClientExitServer(account_id accountID, player_id playerID)
{
	Log.trace("4.client close [accountid:{0}] [player:{1}] .", accountID.value, playerID.value);

	//通知其他模块客户端退出
	auto msg = SerializeUtil::serialize(EMsgID::MSG_S2S_ClientClose);
	binary_writer bw(msg);
	bw<< accountID.value;
	bw<< playerID.value;
	thisModule().Broadcast(msg);

	Log.trace("5.client close [accountid:{0}] [player:{1}] Send to login module .", accountID.value, playerID.value);

	//通知客户端退出

	Log.trace("6.client close [accountid:{0}] [player:{1}] success.", accountID.value, playerID.value);
}

//客户端申请身份认证
void GateLoginHandler::OnRequestLogin(const user_id& userid,uint16_t msgID, binary_reader& data,uint64_t echoid)
{
	PARSE_MESSAGE(C2SReqLogin);
	Log.trace("1.login username[{0}] password[{0}]", tmp.username().data(), tmp.password().data());
	auto serial_num = m_LoginDatas->create_serial_num();

	auto accountID = account_id::create(std::hash<std::string>()(tmp.username()));

	m_LoginDatas->add(serial_num, accountID,userid.get_socket_id());

	Log.trace("2.login username[{0}] password[{1}] accountID[{2}] add to login data", tmp.username().data(), tmp.password().data(), accountID.value);

	//转发给LoginModule
	auto sendMsg = SerializeUtil::serialize(EMsgID::MSG_C2S_REQ_LOGIN);
	binary_writer bw(sendMsg);
	bw << serial_num;
	bw << accountID.value;
	bw << tmp.password();

	auto socketid = userid.get_socket_id();

	if (m_LoginModule == 0)
	{
		m_LoginModule = thisModule().GetOtherModule("login");
		assert(m_LoginModule != 0);
	}

	thisModule().Send(m_LoginModule, sendMsg,
		[this,serial_num, accountID, socketid](const message&msg)
	{
		Log.trace("3.login receive echo accountID[{0}] ", accountID.value);

		binary_reader buf(msg.data(), msg.size());

		uint32_t   _loginRet = 0;
		uint64_t   _serial_num = 0;
		account_id _accountID;

		buf >> _loginRet;
		buf >> _serial_num;
		buf >> _accountID.value;

		S2CLogin_ERet  ret = (S2CLogin_ERet)_loginRet;

		assert(_serial_num == serial_num && _accountID == accountID);

		if (ret == S2CLogin_ERet_Ok)
		{
			auto pLoginModel = m_LoginDatas->find(serial_num);
			if (nullptr == pLoginModel)
			{
				return;
			}

			Log.trace("4.login find login data accountID[{0}] ", accountID.value);

			m_Connects->set_account(pLoginModel->getsocket_id(), accountID);

			Log.trace("5.login add Connection data [{0}--{1}]  ", pLoginModel->getsocket_id(), accountID.value);

			Log.trace("6.login success [accountID{0}]  ", accountID.value);
		}


		S2CLogin s2cLogin;
		s2cLogin.set_accountid(accountID.value);
		s2cLogin.set_ret(ret);

		auto toclient = SerializeUtil::serialize(EMsgID::MSG_S2C_LOGIN_RESULT, s2cLogin);
		m_GateModule->send_socket_message(socketid, toclient);

		//清理流水号
		if (m_LoginDatas->remove(serial_num))
		{
			Log.trace("7.login remove login data [{0}] success", serial_num);
		}
		else
		{
			assert(0);
		}
	});
}

void GateLoginHandler::OnSetPlayerID(const user_id& userid, uint16_t msgID, binary_reader& data,uint64_t echoid)
{
	account_id accountid ;
	player_id playerid ;

	data >> accountid.value;
	data >> playerid.value;

	auto pconn = m_Connects->find_by_account(accountid);
	if (pconn == nullptr)
	{
		Log.trace("OnSetPlayerID:error can not find Connection. ");
		return;
	}

	pconn->setplayer_id(playerid);
	m_Connects->set_player(accountid, playerid);
	
	Log.trace("set [accountid:{0}] playerid:{1}",accountid.value,playerid.value);
}

void GateLoginHandler::OnSetSceneID(const user_id& userid, uint16_t msgID, binary_reader& data,uint64_t echoid)
{
	player_id playerid;
	module_id sceneid;

	data >> playerid.value;
	data >> sceneid.value;
	
	auto pconn = m_Connects->find_by_player(playerid);
	if (pconn == nullptr)
	{
		Log.trace("OnSetSceneID:error can not find Connection. ");
		return;
	}

	pconn->setscene_id(sceneid);
	Log.trace("OnSetSceneID [playerid:{0}] sceneid:{1}", playerid.value, sceneid.value);
}

