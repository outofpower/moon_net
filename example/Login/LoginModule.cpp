#include "LoginModule.h"
#include "AccountDataCache.h"
#include "LoginModuleHandler.h"
#include "LoginHandler.h"

LoginModule::LoginModule(module_id moduleid)
	:actor(moduleid),ModuleBases(this), m_Accounts(nullptr)
{

}

LoginModule::~LoginModule()
{

}

void LoginModule::init(const std::unordered_map<std::string, std::string>& config)
{
	std::string name = config.at("name");
	set_name(name);
}

void LoginModule::on_enter()
{
	actor::on_enter();

	m_Accounts = AddModuleBehaviour<AccountDataCache>();
	assert(m_Accounts != nullptr);
	AddModuleBehaviour<LoginModuleHandler>();
	AddModuleBehaviour<LoginHandler>();
}

void LoginModule::on_exit()
{
	Log.console("LoginModule Exit");
}

void LoginModule::handle_message(const message& msg)
{
	UserContext ctx = get_userdata<UserContext>(msg);
	if (ctx.receiver_echo_id != 0)
	{
		HandlerEchoMessage(ctx.receiver_echo_id, msg);
		return;
	}

	user_id userid;
	userid.set_account_id(account_id::create(ctx.accountid));
	userid.set_player_id(player_id::create(ctx.playerid));

	if (DispatchMessages(userid, msg,ctx.sender_echo_id))
	{
		return;
	}
}


