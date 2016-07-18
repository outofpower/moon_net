#pragma once
#include "../ServerDefine.h"
#include "../ModuleBehaviour.h"
//登陆流程数据
class LoginData
{
public:
	LoginData()
		:_login_serial_num(0)
	{

	}

	ADD_READWRITE(uint64_t, _login_serial_num, login_serial_num);

	ADD_READWRITE(account_id, _account_id, account_id);

	ADD_READWRITE(player_id, _player_id, player_id);

	ADD_READWRITE(socket_id, _socket_id, socket_id);
};

class LoginModelManager:public ModuleBehaviour
{
public:
	LoginModelManager()
		:_login_inc_serial_num(0)
	{

	}

	uint64_t	create_serial_num()
	{
		_login_inc_serial_num++;
		return _login_inc_serial_num;
	}

	void add(uint64_t login_serial_num, uint64_t accountID, socket_id socketID)
	{
		if (_datas.find(login_serial_num) != _datas.end())
		{
			assert(0 && "login data already exist");
			//already exist!.
		}

		LoginData tmp;
		tmp.setlogin_serial_num(login_serial_num);
		tmp.setaccount_id(account_id::create(accountID));
		tmp.setsocket_id(socketID);

		_datas[login_serial_num] = tmp;
	}

	bool remove(uint64_t login_serial_num)
	{
		auto& iter = _datas.find(login_serial_num);
		if (iter != _datas.end())
		{
			_datas.erase(login_serial_num);
			return true;
		}
		else
		{
			return false;
		}
	}

	LoginData* find(uint64_t login_serial_num)
	{
		auto& iter = _datas.find(login_serial_num);
		if (iter != _datas.end())
		{
			return &(iter->second);
		}
		return nullptr;
	}

protected:
	std::unordered_map<uint64_t, LoginData>		_datas;
	uint64_t																_login_inc_serial_num;
};