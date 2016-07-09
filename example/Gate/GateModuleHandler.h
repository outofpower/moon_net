#pragma once
#include "../ModuleBehaviour.h"

//处理模块之间的消息
class GateModuleHandler :public ModuleBehaviour
{
public:
	GateModuleHandler(ModuleBases& module);
	~GateModuleHandler();

	bool Init() override;

protected:
	//服务器启动
	void OnServerStart(const user_id& userid, uint16_t msgID, buffer_reader& data,uint64_t echoid);
	//某个模块启动
	void OnModuleStart(const user_id& userid,uint16_t msgID, buffer_reader& data,uint64_t echoid);
};

