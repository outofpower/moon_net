#pragma once
#include "ServerDefine.h"
#include "Component.h"
class ModuleBases;
class ModuleBehaviour
{
public:
	ModuleBehaviour(ModuleBases& module)
		:m_module(module)
	{
	}

	virtual ~ModuleBehaviour()
	{
		for (auto& iter : m_Components)
		{
			delete iter.second;
		}
	}

	const std::string& GetName() { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	virtual bool Init() { return true; }

	virtual void OnEnter() {};

	virtual void OnExit() {};

	virtual void OnClientClose(account_id accountID, player_id playerID)
	{
		for (auto& iter : m_Components)
		{
			iter.second->OnClientClose(accountID, playerID);
		}
	}

	template<class TComponent>
	TComponent* AddComponent(const std::string& name);

	template<class TComponent>
	TComponent* GetComponent(const std::string& name);

	ModuleBases& thisModule(){return m_module;}
private:
	ModuleBases&													m_module;
	std::string																 m_name;

	std::unordered_map<std::string, Component*> m_Components;
};


template<class TComponent>
inline TComponent * ModuleBehaviour::AddComponent(const std::string & name)
{
	auto iter = m_Components.emplace(name, new TComponent());
	assert(iter.second&&"The component is already exist!");
	auto& t = iter.first->second;
	t->SetName(name);
	t->SetOwner(this);
	t->SetModule(m_module);
	if (t->Init())
	{
		return dynamic_cast<TComponent*>(t);
	}
	else
	{
		return nullptr;
	}
}


template<class TComponent>
inline TComponent * ModuleBehaviour::GetComponent(const std::string & name)
{
	auto& iter = m_Components.find(name);
	if (iter != m_Components.end())
	{
		return dynamic_cast<TComponent*>(iter->second);
	}
	return nullptr;
}
