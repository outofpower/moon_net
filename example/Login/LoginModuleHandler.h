#pragma once
#include "../ModuleBehaviour.h"

class LoginModuleHandler :public ModuleBehaviour
{
public:
	LoginModuleHandler(ModuleBases& module);
	~LoginModuleHandler();

	bool Init() override;

protected:
	void OnServerStart(const user_id& userid, uint16_t msgID, buffer_reader& data,uint64_t echoid);

	void OnModuleStart(const user_id& userid, uint16_t msgID, buffer_reader& data,uint64_t echoid);
};

