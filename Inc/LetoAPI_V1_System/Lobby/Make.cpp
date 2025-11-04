#include "Make.hpp"

#include <cstdio>
#include <cstdarg>

// ====================================================================================================

#include "LobbyManager.hpp"

uint32_t GetLobbiesNear(LobbyInfo_V1* info, uint32_t available)
{
	return LobbyManager_V1::Instance().GetLobbiesNear(info, available);
}

bool CreateLobby(LobbyConnection_V1* lobby, uint16_t app_id, uint8_t max_count, LobbyConnection_V1_Callback callback)
{
	return LobbyManager_V1::Instance().CreateLobby(lobby, app_id, max_count, callback);
}

void JoinLobby(LobbyInfo_V1 info, LobbyConnection_V1_Callback callback)
{
	LobbyManager_V1::Instance().JoinLobby(info, callback);
}

bool GetActiveLobby(LobbyConnection_V1* lobby)
{
	return LobbyManager_V1::Instance().GetActiveLobby(lobby);
}

void QuitLobby()
{
	LobbyManager_V1::Instance().QuitLobby();
}

void DisconnectMember(uint32_t id)
{
	LobbyManager_V1::Instance().DisconnectMember(id);
}

void SendData(const void* data, uint32_t size)
{
	LobbyManager_V1::Instance().SendData(data, size);
}

// ====================================================================================================

const LobbyAPI_V1* Make_LobbyAPI()
{
	static const LobbyAPI_V1 api
	{
		&GetLobbiesNear,
		&CreateLobby,
		&JoinLobby,
		&GetActiveLobby,
		&QuitLobby,
		&DisconnectMember,
		&SendData
	};

    return &api;
}
