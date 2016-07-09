#pragma  once

enum class EMsgID
{
	MSG_UNKNOWN,
	MSG_C2S_REQ_LOGIN,
	MSG_S2C_LOGIN_RESULT,

	MSG_C2S_REQ_CREATE_PLAYER,
	MSG_S2C_CREATE_PLAYER,

	MSG_C2S_REQ_PLAYER_DATA,
	MSG_S2C_NO_PLAYER,
	MSG_S2C_PLAYER_DATA,

	MSG_MUST_HAVE_PLAYERID,

	MSG_S2S_PLAYER_ENTER_SCENE,
	MSG_S2S_PLAYER_EXIT_SCENE,
	MSG_S2S_SET_ACCOUNT_PLAYERID,
	MSG_S2S_SET_PLAYER_SCENEID,
	MSG_S2S_ClientClose,
	MSG_S2S_SERVER_START,
	MSG_S2S_MODULE_START
};



#define PARSE_MESSAGE(Class)\
	if (data.size() == 0)\
		return;\
	Class tmp;\
	if (!tmp.ParseFromArray(data.data(), data.size()&0xFFFFFFFF))\
	{\
		return;\
	}

#define  CHECK_PLAYER(playerid)\
	PlayerData* playerdata = m_Manager.GetPlayerDatas()->find(playerid);\
	if (nullptr == playerdata)\
	{\
		assert(0);\
		return;\
	}
