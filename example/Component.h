#pragma once
#include "ServerDefine.h"

class ModuleBehaviour;
class ModuleBases;

class Component :noncopyable
{
public:
	Component()
		:m_Owner(nullptr),m_module(nullptr){}

	virtual ~Component(){};

	const std::string& GetName() { return m_name; }
	
	ModuleBehaviour* GetOwner(){return m_Owner;}

	ModuleBases& thisModule() { return *m_module; }
	
protected:
	void SetName(const std::string& name) { m_name = name; }
	void SetOwner(ModuleBehaviour* owner) { m_Owner = owner; }
	void SetModule(ModuleBases* module) { m_module = module; }
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
