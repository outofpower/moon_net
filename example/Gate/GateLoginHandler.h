#pragma once
#include "../ModuleBehaviour.h"

class LoginModelManager;
class ConnectionManager;
class GateModule;
class GateLoginHandler :public ModuleBehaviour
{
public:
	GateLoginHandler(ModuleBases& module);

	~GateLoginHandler();

	bool Init() override;

protected:
	void	OnClientClose(account_id accountID, player_id playerID) override;

	void OnRequestLogin(const user_id& userid,uint16_t msgID, buffer_reader& data,uint64_t echoid);

	void OnSetPlayerID(const user_id& userid,uint16_t msgID, buffer_reader& data,uint64_t echoid);

	void OnSetSceneID(const user_id& userid,uint16_t msgID, buffer_reader& data,uint64_t echoid);
protected:
	void ClientExitServer(account_id accountID, player_id playerID);

private:
	LoginModelManager*  m_LoginDatas;
	ConnectionManager*   m_Connects;
	module_id                     m_LoginModule;
	GateModule*				  m_GateModule;
};

