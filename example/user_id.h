#pragma once
#include <base_id.h>
#include <cassert>
#include <unordered_map>
#include <socket_id.h>

struct account_id:public base_id<uint64_t,account_id>
{
	
};

namespace std
{
	template<>
	struct hash<account_id>
	{
		std::size_t operator()(const account_id& k) const
		{
			return (size_t)k.value;
		}
	};
};

struct unique_id :public base_id<uint64_t, unique_id>
{
};

namespace std
{
	template<>
	struct hash<unique_id>
	{
		std::size_t operator()(const unique_id& k) const
		{
			return (size_t)k.value;
		}
	};
};


struct player_id :public base_id<uint64_t, player_id>
{
	player_id& operator=(const unique_id& other)
	{
		value = other.value;
		return *this;
	}
};

namespace std
{
	template<>
	struct hash<player_id>
	{
		std::size_t operator()(const player_id& k) const
		{
			return (size_t)k.value;
		}
	};
};

class user_id
{
public:
	user_id()
	{

	}

	user_id(account_id id)
	{
		accountid = id;
	}

	user_id(player_id id)
	{
		playerid = id;
	}

	user_id(const user_id& other)
		:accountid(other.accountid), playerid(other.playerid)
	{

	}

	user_id& operator=(const user_id& other)
	{
		accountid = other.accountid;
		playerid    = other.playerid;
		return *this;
	}

	bool has_account_id() const
	{
		return accountid.value !=0;
	}

	bool has_player_id() const
	{
		return playerid.value !=0;
	}

	player_id get_player_id() const
	{
		assert(has_player_id() && "has no playerid!!");
		return playerid;
	}

	account_id get_account_id() const
	{
		assert(has_account_id() && "has no accountid!!");
		return accountid;
	}

	moon::socket_id get_socket_id()const
	{
		return sockid;
	}

	void set_player_id(player_id v)
	{
		playerid = v;
	}

	void set_account_id(account_id v)
	{
		accountid = v;
	}

	void set_socket_id(moon::socket_id v)
	{
		sockid = v;
	}

	operator account_id()  const
	{
		return get_account_id();
	}

	operator player_id() const
	{
		return get_player_id();
	}

private:
	account_id				accountid;
	player_id					playerid;
	moon::socket_id		sockid;
};

