#include "ModuleBases.h"

ModuleBases::ModuleBases(moon::actor* value)
	:m_actor(value)
{
}

ModuleBases::~ModuleBases()
{
	for (auto& iter : m_ModuleBehaviours)
	{
		delete iter.second;
	}
}

void ModuleBases::AddOtherModule(const std::string & name, module_id id)
{
	m_otherModules[name] = id;
	if (name == "gate")
	{
		_GateModule = id;
	}
}

module_id ModuleBases::GetOtherModule(const std::string & name)
{
	auto& iter = m_otherModules.find(name);
	if (iter != m_otherModules.end())
	{
		return iter->second;
	}
	return module_id::create(0);
}

void ModuleBases::Send(module_id receiver, const buffer_ptr & data, EchoHandler && hander, EMessageType type)
{
	auto cur_tm = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto inc_id = ++m_echoIncID;
	uint64_t echo_msg_id = (cur_tm << 32 | inc_id);

	message msg(data);
	msg.set_type(type);
	UserContext ctx;
	ctx.sender_echo_id = echo_msg_id;
	msg.set_userdata((uint8_t*)&ctx, sizeof(ctx));

	m_echoHandlers.emplace(echo_msg_id, std::forward<EchoHandler>(hander));

	m_actor->send_message(receiver, msg);
}

void ModuleBases::Send(module_id receiver, const buffer_ptr & data, EMessageType type, uint64_t echoid)
{
	message msg(data);
	msg.set_type(type);

	UserContext ctx;
	ctx.receiver_echo_id = echoid;
	msg.set_userdata((uint8_t*)&ctx, sizeof(ctx));

	m_actor->send_message(receiver, msg);
}

void ModuleBases::Broadcast(const buffer_ptr & data, EMessageType type)
{
	message msg(data);
	msg.set_sender(m_actor->get_id());
	msg.set_type(type);
	m_actor->broadcast_message(msg);
}

void ModuleBases::Send(account_id accountid, const buffer_ptr & data)
{
	message msg(data);
	msg.set_type(EMessageType::ToClient);

	UserContext ctx;
	ctx.accountid = accountid;
	msg.set_userdata((uint8_t*)&ctx, sizeof(ctx));

	m_actor->send_message(_GateModule, msg);
}

void ModuleBases::Send(player_id playerid, const buffer_ptr & data)
{
	message msg(data);
	msg.set_type(EMessageType::ToClient);

	UserContext ctx;
	ctx.playerid = playerid;
	msg.set_userdata((uint8_t*)&ctx, sizeof(ctx));
	m_actor->send_message(_GateModule, msg);
}

void ModuleBases::Send(const std::vector<player_id>& players, const buffer_ptr & data)
{
	for (auto& id : players)
	{
		Send(id, data);
	}
}

bool ModuleBases::DispatchMessages(const user_id & userid, const message & msg, uint64_t sender_echo_id)
{
	buffer_reader br(msg.data(), msg.size());

	uint16_t msgID = 0;
	br >> msgID;

	if (msgID == uint16_t(EMsgID::MSG_S2S_ClientClose))
	{
		account_id accountID;
		player_id playerID;
		br >> accountID.value;
		br >> playerID.value;
		OnClientClose(accountID, playerID);
		return true;
	}

	auto iter = m_msgFuncs.find(msgID);
	if (iter != m_msgFuncs.end())
	{
		iter->second(userid, msgID, br, sender_echo_id);
		return true;
	}
	return false;
}

moon::timer_pool & ModuleBases::GetTimerPool()
{
	return m_actor->get_timer_pool();
}

void ModuleBases::HandlerEchoMessage(uint64_t echo_id, const message & msg)
{
	auto& iter = m_echoHandlers.find(echo_id);
	if (iter != m_echoHandlers.end())
	{
		iter->second(msg);
		m_echoHandlers.erase(iter);
	}
}

void ModuleBases::OnClientClose(account_id accountID, player_id playerID)
{
	for (auto& mbh : m_ModuleBehaviours)
	{
		mbh.second->OnClientClose(accountID, playerID);
	}
}
