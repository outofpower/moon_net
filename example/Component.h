#pragma once
#include "ServerDefine.h"

class ModuleBehaviour;
class ModuleBases;

class Component
{
public:
	Component()
		:m_Owner(nullptr),m_module(nullptr){}

	virtual ~Component(){};

	const std::string& GetName() { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	void SetOwner(ModuleBehaviour* owner){m_Owner = owner;}
	ModuleBehaviour* GetOwner(){return m_Owner;}

	void SetModule(ModuleBases* module) { m_module = module; }

protected:
	ModuleBases& thisModule() { return *m_module; }

//override
public:
	virtual bool Init() { return false; }
	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual void OnClientClose(account_id accountID, player_id playerID) {};

protected:
	std::string																 m_name;
	ModuleBehaviour*												 m_Owner;
	ModuleBases*													     m_module;
};
