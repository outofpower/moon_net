#pragma once
#include "../ServerDefine.h"
#include "../ModuleBehaviour.h"

DECLARE_SHARED_PTR(Connection)

class Connection
{
public:
	Connection()
	{

	}

	~Connection()
	{

	}

	ADD_READWRITE(account_id, _account_id, account_id);

	ADD_READWRITE(player_id, _player_id, player_id);

	ADD_READWRITE(socket_id, _socket_id, socket_id);

	ADD_READWRITE(module_id, _scene_id, scene_id);

	ADD_READWRITE(std::string, _account_name, account_name);
};

class ConnectionManager:public ModuleBehaviour
{
public:
	ConnectionManager()
	{

	}

	void	set_account(socket_id socketid,account_id accountid)
	{
		auto conn = std::make_shared<Connection>();
		conn->setaccount_id(accountid);
		conn->setsocket_id(socketid);

		{
			auto& iter = _datas.find(socketid);
			if (iter != _datas.end())
			{
				assert(0);
			}
		}

		_datas.emplace(socketid, conn);

		{
			auto& iter = _account_datas.find(accountid);
			if (iter != _account_datas.end())
			{
				assert(0);
			}
		}

		_account_datas.emplace(accountid, conn);
	}

	void set_player(account_id accountid, player_id playerid)
	{
		auto conn  = find_by_account(accountid);
		if (conn != nullptr)
		{
			conn->setplayer_id(playerid);
			_player_datas.emplace(playerid, conn);
		}
	}

	bool remove(socket_id socketid)
	{
		auto& iter = _datas.find(socketid);
		if (iter != _datas.end())
		{
			account_id accountid;
			player_id playerid;

			accountid = iter->second->getaccount_id();
			playerid = iter->second->getplayer_id();

			_account_datas.erase(accountid);
			_player_datas.erase(playerid);
			_datas.erase(iter);
			return true;
		}
		return false;
	}

	bool remove_by_player(player_id playerid)
	{
		auto& iter = _player_datas.find(playerid);
		if (iter != _player_datas.end())
		{
			account_id accountid;
			socket_id socketid;

			accountid = iter->second->getaccount_id();
			socketid = iter->second->getsocket_id();

			_account_datas.erase(accountid);
			_datas.erase(socketid);
			_player_datas.erase(iter);
			return true;
		}
		return false;
	}

	bool remove_by_account(account_id accountid)
	{
		auto& iter = _account_datas.find(accountid);
		if (iter != _account_datas.end())
		{
			player_id playerid;
			socket_id socketid;

			playerid = iter->second->getplayer_id();
			socketid = iter->second->getsocket_id();

			_player_datas.erase(playerid);
			_datas.erase(socketid);
			_account_datas.erase(iter);
			return true;
		}
		return false;
	}


	Connection_ptr find(socket_id socketid)
	{
		auto& iter = _datas.find(socketid);
		if (iter != _datas.end())
		{
			return (iter->second);
		}
		return nullptr;
	}

	Connection_ptr find_by_account(account_id accountid)
	{
		auto& iter = _account_datas.find(accountid);
		if (iter != _account_datas.end())
		{
			return (iter->second);
		}
		return nullptr;
	}

	Connection_ptr find_by_player(player_id playerid)
	{
		auto& iter = _player_datas.find(playerid);
		if (iter != _player_datas.end())
		{
			return (iter->second);
		}
		return nullptr;
	}

protected:
	// socketid-Connection
	std::unordered_map<socket_id, Connection_ptr>		_datas;
	std::unordered_map<account_id, Connection_ptr>	_account_datas;
	std::unordered_map<player_id, Connection_ptr>		_player_datas;
};

