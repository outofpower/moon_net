#pragma once
#include "../ServerDefine.h"
#include "../ModuleBehaviour.h"
struct AccountData
{
	std::string						username;
	std::string						password;
	std::string						address;
	account_id					accountID;
	bool								bOnline;
};

class AccountDataCache:public ModuleBehaviour
{
public:
	AccountDataCache();
	~AccountDataCache(void);

	bool Init() override;
	//根据用户名查找账号ID, 返回值 0 未找到
	//uint32_t	FindAccountID(const std::string& username);
	bool			IsExistAccount(account_id accountID);
	bool			FindAccountInfo(account_id accountID, AccountData& info);
	//判断账号密码是否匹配
	bool			Match(account_id, const std::string& password);
	//判断是否已经登陆
	bool			IsOnline(account_id);
	//账号上线
	bool			OnLine(account_id accountID);
	//账号下线
	bool			OffLine(account_id accountID);
	//添加账号
	bool			AddAccount(account_id accountID,const std::string& username, const std::string& password);

private:
	std::unordered_map<account_id, AccountData>		m_AccountMap;
};



