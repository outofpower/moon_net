#pragma once
#include <actor.h>
#include "../ModuleBases.h"

using namespace moon;

class AccountDataCache;

//хож╓дё©И
class LoginModule:
	public actor,
	public ModuleBases
{
public:
	LoginModule(module_id moduleid);
	~LoginModule();

	void init(const std::unordered_map<std::string, std::string>& config) override;

	void on_enter() override;

	void on_exit() override;

	void handle_message(const message& msg) override;

private:
	AccountDataCache*  m_Accounts;
};

