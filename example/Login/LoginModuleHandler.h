#pragma once
#include "../ModuleBehaviour.h"

class LoginModuleHandler :public ModuleBehaviour
{
public:
	LoginModuleHandler();
	~LoginModuleHandler();

	bool Init() override;

protected:
	void OnServerStart(const user_id& userid, uint16_t msgID, binary_reader& data,uint64_t echoid);

	void OnModuleStart(const user_id& userid, uint16_t msgID, binary_reader& data,uint64_t echoid);
};

