#pragma once
#include <broker.h>
#include "../ModuleBases.h"

using namespace moon;

class ConnectionManager;

//Íø¹ØÄ£¿é
class GateModule:
	public broker,
	public ModuleBases
{
public:
	GateModule(module_id moduleid);
	~GateModule();

public:
	void init(const std::unordered_map<std::string,std::string>& config) override;

	void on_enter() override;

	void on_exit() override;

	void handle_message(const message& msg) override;
public:
	void ForceCloseClient(socket_id sockid);
protected:
	void ClientConnect(const message& msg);

	void ClientClose(const message& msg);

	void ClientData(const message& msg);

	void ActorData(const message& msg);

	void ToClientData(const message& msg);
protected:
	ConnectionManager*		m_Connections;
	module_id                       m_WorldModule;
};

