#pragma once
#include <cstdint>

#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

struct UserContext
{
	UserContext()
		:accountid(0), playerid(0), sender_echo_id(0),receiver_echo_id(0)
	{

	}
	uint64_t	accountid;
	uint64_t	playerid;
	uint64_t   sender_echo_id;
	uint64_t   receiver_echo_id;
};

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif
