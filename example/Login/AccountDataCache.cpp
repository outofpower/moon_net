#include "AccountDataCache.h"

AccountDataCache::AccountDataCache(ModuleBases& module)
	:ModuleBehaviour(module)
{
}


AccountDataCache::~AccountDataCache(void)
{
}

bool AccountDataCache::Init()
{
	return true;
}

bool	AccountDataCache::IsExistAccount(account_id accountID)
{
	auto iter = m_AccountMap.find(accountID);
	if (iter != m_AccountMap.end())
	{
		return true;
	}
	return false;
}

bool AccountDataCache::FindAccountInfo(account_id accountID, AccountData& info)
{
	auto iter = m_AccountMap.find(accountID);
	if (iter != m_AccountMap.end())
	{
		info =  iter->second;
		return true;
	}
	return false;
}

bool AccountDataCache::Match(account_id accountID, const std::string& password )
{
	AccountData info;
	if (FindAccountInfo(accountID, info))
	{
		return (info.password == password);
	}
	return false;
}

bool AccountDataCache::IsOnline(account_id accountID)
{
	auto iter = m_AccountMap.find(accountID);
	if (iter != m_AccountMap.end())
	{
		return iter->second.bOnline;
	}
	return false;
}

bool AccountDataCache::OnLine(account_id accountID)
{
	auto iter = m_AccountMap.find(accountID);
	if (iter != m_AccountMap.end())
	{
		iter->second.bOnline = true;
		return true;
	}
	return false;
}

bool AccountDataCache::OffLine(account_id accountID)
{
	auto iter = m_AccountMap.find(accountID);
	if (iter != m_AccountMap.end())
	{
		iter->second.bOnline = false;
		return true;
	}
	return false;
}

bool AccountDataCache::AddAccount(account_id accountID, const std::string& username, const std::string& password )
{
	if (IsExistAccount(accountID))
	{
		return false;
	}

	AccountData info;
	info.username	 = username;
	info.password	 = password;
	info.bOnline     = false;
	info.accountID	 = accountID;
	m_AccountMap[info.accountID] = info;
	return true;
}
