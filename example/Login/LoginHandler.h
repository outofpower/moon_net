#pragma once
#include "../ModuleBehaviour.h"

class AccountDataCache;
//��½�����Ϣ����
class LoginHandler:public ModuleBehaviour
{
public:
	LoginHandler();
	~LoginHandler(void);

	bool Init() override;

	void	OnClientClose(account_id accountID, player_id playerID) override;
private:
	//�ͻ��������½
	void OnRequestLogin(const user_id& userid, uint16_t msgID, binary_reader& data,uint64_t echoid);

private:
	AccountDataCache*  m_Accounts;
};

