#pragma once
#include "../ModuleBehaviour.h"

//����ģ��֮�����Ϣ
class GateModuleHandler :public ModuleBehaviour
{
public:
	GateModuleHandler();
	~GateModuleHandler();

	bool Init() override;

protected:
	//����������
	void OnServerStart(const user_id& userid, uint16_t msgID, binary_reader& data,uint64_t echoid);
	//ĳ��ģ������
	void OnModuleStart(const user_id& userid,uint16_t msgID, binary_reader& data,uint64_t echoid);
};

