#include "GateModule.h"
#include "GateModuleHandler.h"
#include "GateLoginHandler.h"
#include "LoginData.h"
#include "Connection.h"

GateModule::GateModule(module_id moduleid)
	:broker(moduleid),ModuleBases(this), m_Connections(nullptr)
{
	
}

GateModule::~GateModule()
{

}

void GateModule::init(const std::unordered_map<std::string, std::string>& config)
{
	std::string name = config.at("name");
	std::string ip = config.at("ip");
	std::string port = config.at("port");
	std::string thread_num = config.at("net_thread_num");

	set_name(name);

	init_net(std::stoi(thread_num));

	listen(ip, port);
}

void GateModule::on_enter()
{
	broker::on_enter();

	m_Connections = AddModuleBehaviour<ConnectionManager>();
	AddModuleBehaviour<LoginModelManager>();
	AddModuleBehaviour<GateModuleHandler>();
	AddModuleBehaviour<GateLoginHandler>();
}

void GateModule::on_exit()
{
	broker::on_exit();
	Log.console("GateModule Exit");
}

void GateModule::handle_message(const message& msg)
{
	switch (msg.get_type())
	{
	case EMessageType::SocketConnect:
	{
		ClientConnect(msg);
		break;
	}
	case EMessageType::SocketData:
	{
		ClientData(msg);
		break;
	}
	case EMessageType::SocketClose:
	{
		ClientClose(msg);
		break;
	}
	case EMessageType::ActorData:
	{
		ActorData(msg);
		break;
	}
	case EMessageType::ToClient:
	{
		ToClientData(msg);
		break;
	}
	}
}

void GateModule::ForceCloseClient(socket_id sockid)
{
	//Send to client close.
	force_close_socket(sockid);
}

void GateModule::ClientConnect(const message& msg)
{
	buffer_reader buf(msg.data(), msg.size());

	std::string addr;
	buf >> addr;
	Log.trace("socket [socket:{0}] [address:{1}] connect", msg.get_socket_id().value,addr.data());
}

void GateModule::ClientClose(const message& msg)
{
	buffer_reader buf(msg.data(), msg.size());

	std::string addr;
	buf >> addr;

	Log.trace("1.client close [socketid:{0}] [address:{1}] begin.", msg.get_socket_id().value, addr.data());
	auto pconn = m_Connections->find(msg.get_socket_id());
	if (pconn == nullptr)
	{
		Log.trace("!!client close failed can not find conn [socketid:{0}].", msg.get_socket_id().value);
		return;
	}

	auto accountID = pconn->getaccount_id();
	auto playerID = pconn->getplayer_id();

	Log.trace("3.client close [socketid:{0}] [address:{1}] find account data[accountid:{2}]  success[playerid:{3}].", msg.get_socket_id().value, addr.data(), accountID.value,playerID.value);
	
	OnClientClose(accountID, playerID);
	
	if (m_Connections->remove(msg.get_socket_id()))
	{
		Log.trace("client close [socketid:{0}] [address:{1}]  accountID[{2}] playerID[{3}] end.", msg.get_socket_id().value, addr.data(), accountID.value, playerID.value);
	}
	else
	{
		assert(0);
	}
}

//客户端发来的数据
void GateModule::ClientData(const message& msg)
{
	if (msg.size() == 0)
		return;

	user_id id(account_id::create(0));
	id.set_socket_id(msg.get_socket_id());

	if (DispatchMessages(id,msg,0))
	{
		return;
	}
	
	auto pconn = m_Connections->find(msg.get_socket_id());
	if (pconn == nullptr)
	{
		Log.trace("非法数据!");
		return;
	}


	uint16_t msgID = *(uint16_t*)msg.data();
	if (msgID > (uint16_t)EMsgID::MSG_MUST_HAVE_PLAYERID)
	{
		if (pconn->getplayer_id() == player_id())
		{
			Log.trace("非法数据!");
			return;
		}
	}

	UserContext ctx;
	ctx.accountid = pconn->getaccount_id();
	ctx.playerid = pconn->getplayer_id();

	msg.set_userdata((uint8_t*)&ctx,sizeof(ctx));

	if (pconn->getscene_id() != 0)
	{
		//如果在玩家在场景模块中 则发送给场景模块
	}
	else
	{
		//否则发送给 world 模块
		if (m_WorldModule == 0)
		{
			m_WorldModule = GetOtherModule("world");
			//assert(m_WorldModule != 0);
		}
	}
}

void GateModule::ActorData(const message& msg)
{
	UserContext ctx = get_userdata<UserContext>(msg);
	if (ctx.receiver_echo_id != 0)
	{
		HandlerEchoMessage(ctx.receiver_echo_id, msg);
		return;
	}

	user_id userid;
	userid.set_account_id( account_id::create(ctx.accountid));
	userid.set_player_id(player_id::create(ctx.playerid));
	userid.set_socket_id(socket_id::create(msg.get_socket_id()));
	if (DispatchMessages(userid,msg,ctx.sender_echo_id))
	{
		return;
	}
}

void GateModule::ToClientData(const message& msg)
{
	UserContext ctx = get_userdata<UserContext>(msg);

	Connection_ptr pconn = nullptr;

	if (ctx.accountid != 0)
	{
		pconn = m_Connections->find_by_account(account_id::create(ctx.accountid));
	}
	else if (ctx.playerid != 0)
	{
		pconn = m_Connections->find_by_player( player_id::create(ctx.playerid));
	}

	if (pconn == nullptr)
	{
		return;
	}

	send_socket_message(pconn->getsocket_id(), msg.msg_data());
}


