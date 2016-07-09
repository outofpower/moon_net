#pragma once
#include <message.h>
#include <actor.h>
#include "ServerDefine.h"
#include "ModuleBehaviour.h"
#include "MessageID.h"
#include "UserContext.h"
#include "SerializeUtil.h"

using EchoHandler = std::function<void(const message&)>;
using MessageFunc = std::function<void(const user_id& userid, uint16_t, buffer_reader&, uint64_t)>;

class ModuleBases
{
public:
	ModuleBases(moon::actor* value);
	virtual ~ModuleBases();

	void						AddOtherModule(const std::string& name, module_id id);

	module_id			GetOtherModule(const std::string& name);

	void						Send(module_id receiver, const buffer_ptr& data, EchoHandler&& hander, EMessageType type = EMessageType::ActorData);

	void						Send(module_id receiver, const buffer_ptr& data, EMessageType type = EMessageType::ActorData, uint64_t echoid = 0);

	void						Broadcast(const buffer_ptr& data, EMessageType type = EMessageType::ActorData);

	void						Send(account_id accountid, const buffer_ptr& data);

	void						Send(player_id playerid, const buffer_ptr& data);

	void						Send(const std::vector<player_id>& players, const buffer_ptr& data);

	//消息分发
	bool						DispatchMessages(const user_id& userid, const message& msg, uint64_t sender_echo_id);

	moon::timer_pool& GetTimerPool();

	const module_id	 GetID() const { return m_actor->get_id(); };

	const std::string& GetName() const { return m_actor->get_name(); };

	template<class T>
	T							get_userdata(const message& msg);

	template<class TModuleBehaviour>
	TModuleBehaviour * AddModuleBehaviour();

	template<class TModuleBehaviour>
	TModuleBehaviour * GetModuleBehaviour();

	template<class T, class Func>
	void						RegisterMessage(EMsgID msgID, T* t, Func f);

	ADD_READONLY(module_id, _GateModule, GateModule);
protected:
	void						HandlerEchoMessage(uint64_t echo_id, const message& msg);
	void						OnClientClose(account_id accountID, player_id playerID);
protected:
	std::unordered_map<std::string, module_id>						m_otherModules;
	std::unordered_map<uint64_t, EchoHandler>						m_echoHandlers;
	uint32_t																					m_echoIncID;
	moon::actor*																			m_actor;
	std::unordered_map<uint16_t, MessageFunc>						m_msgFuncs;
	std::unordered_map<std::string, ModuleBehaviour*>			m_ModuleBehaviours;
};

template<class T>
inline T ModuleBases::get_userdata(const message & msg)
{
	T tmp;
	auto data = msg.get_userdata(sizeof(T));
	if (data != nullptr)
	{
		tmp = *(T*)data;
	}
	return tmp;
}

template<class TModuleBehaviour>
inline TModuleBehaviour * ModuleBases::AddModuleBehaviour()
{
	auto t = new TModuleBehaviour(*this);
	do 
	{	
		BREAK_IF(t == nullptr || !t->Init());
		if (t != nullptr && t->Init())
		{
			auto name = typeid(TModuleBehaviour).name();
			t->SetName(name);
			auto iter = m_ModuleBehaviours.emplace(name, t);
			BREAK_IF(!iter.second);
			return t;
		}
	} while (0);
	SAFE_DELETE(t);
	return nullptr;
}

template<class TModuleBehaviour>
inline TModuleBehaviour * ModuleBases::GetModuleBehaviour()
{
	auto& iter = m_ModuleBehaviours.find(typeid(TModuleBehaviour).name());
	if (iter != m_ModuleBehaviours.end())
	{
		return dynamic_cast<TModuleBehaviour*>(iter->second);
	}
	return nullptr;
}

template<class T, class Func>
inline void ModuleBases::RegisterMessage(EMsgID msgID, T * t, Func f)
{
	m_msgFuncs[uint16_t(msgID)] = std::bind(f, t, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}


