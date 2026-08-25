#include "WebDevicesTask.hpp"

#include <GamesSupport/GameLoader.hpp>
#include <Auth/AuthHandler.hpp>
#include <Time/TimeUtils.hpp>

#include <LetoAPI_V1_System/Web/WebDevicesList.hpp>
#include <LetoAPI_V1_System/Web/WebManager.hpp>

#include <LetoAPI_V1_System/Lobby/LobbyManager.hpp>

#include <System/DeviceID.hpp>

enum DEVICE_INFO_TYPE
{
	INFO_NAME_0_6 = 0,
	INFO_NAME_7_12,
	INFO_ID_ACTIVE_LOBBY,
	INFO_AVATAR_JOIN_LOBBY,
	INFO_FLAGS,
	INFO_MAX
};

bool WebDevicesTask::UpdateInfo()
{
	return AuthHandler::Instance().GetCurrentAccount(account);
}

void WebDevicesTask::SendAccountMsg()
{
	uint8_t u_data[10]{};
	uint32_t device_id = GetDeviceID();
	uint32_t lobby_owner = 0, join_lobby = 0;

	LobbyConnection_V1 lobby;
	if (LobbyManager_V1::Instance().GetActiveLobby(&lobby))
		lobby_owner = lobby.owner;
	
	if (LobbyManager_V1::Instance().GetJoiningLobby(&lobby))
		join_lobby = lobby.owner;

	// Уведомление, что устройство активно и готово 
	u_data[0] = 0x00;

	self_info.channel = WC_CHANNEL_ALL;
	self_info.id = device_id;
	self_info.avatar_id = account.Avatar;
	self_info.lobby_owner = lobby_owner;
	self_info.joining_lobby = join_lobby;
	memset(self_info.web_name, 0, sizeof(self_info.web_name));
	memcpy(self_info.web_name, &account.Name[0], 10);

	self_info.flags &=~ WD_FLAG_READY;
	if (LobbyManager_V1::Instance().GetReady())
		self_info.flags |= WD_FLAG_READY;

	for (uint8_t info = 0; info < INFO_MAX; ++info)
	{
		memset(u_data + 2, 0, 6);
		u_data[1] = info;

		switch (info)
		{
		case INFO_NAME_0_6:
			memcpy(u_data + 2, &account.Name[0], 6);
			break;
		case INFO_NAME_7_12:
			memcpy(u_data + 2, &account.Name[6], 6);
			break;
		case INFO_ID_ACTIVE_LOBBY:
			memcpy(u_data + 2, &self_info.id, 4);
			memcpy(u_data + 6, &self_info.lobby_owner, 4);
			break;
		case INFO_AVATAR_JOIN_LOBBY:
			memcpy(u_data + 2, &self_info.avatar_id, 4);
			memcpy(u_data + 6, &self_info.joining_lobby, 4);
			break;
		case INFO_FLAGS:
			memcpy(u_data + 2, &self_info.flags, 1);
			break;
		}


		WebManager_V1::Instance().SendData(connection, u_data, sizeof(u_data));
	}
}

void WebDevicesTask::SendAppMsg()
{
	if (!CurrentLoadedGame)
		return;
	
	uint8_t u_data[8]{};

	// Уведомление, что устройство активно и находится в приложении
	u_data[0] = 0x03;

	uint16_t id = CurrentLoadedGame->id;
	u_data[1] = id & 0xFF;
	u_data[2] = (id >> 8) & 0xFF;

	self_info.app_id = id;

	WebManager_V1::Instance().SendData(connection, u_data, sizeof(u_data));
}

void WebDevicesTask::ClearSelfInfo()
{
	memset(&self_info, 0, sizeof(self_info));
}

void WebDevicesTask::SaveSelfInfo()
{
	WebManager_V1::Instance().SetSelfInfo(self_info);
}

void WebDevicesTask::RefreshDevicesList()
{
	const uint32_t ACTIVITY_TIMEOUT = 4000;

	if (!refresh_timer.Expired())
		return;
	refresh_timer.Start();

	WebDevicesList.RemoveIf([&](const WebDeviceInfo_ListItem& info) {
		return TimeUtils::GetCurrentMs() - info.last_activity > ACTIVITY_TIMEOUT;
	});

	for (WebDeviceInfo_ListItem& info : WebDevicesList)
	{
		if (TimeUtils::GetCurrentMs() - info.last_app_activity > ACTIVITY_TIMEOUT)
		{
			info.device.app_id = 0;
		}
	}
}

bool WebDevicesTask::Do()
{
	if (!inited) return true;

	RefreshDevicesList();
	ClearSelfInfo();
	if (UpdateInfo())
	{
		SendAccountMsg();
		SendAppMsg();
	}
	SaveSelfInfo();

	return true;
}

#include <VirtualConsole/VirtualConsole.hpp>

static void WebDevTask_Callback(uint8_t channel, uint8_t port, uint32_t id, const void* data, uint32_t size)
{
	if (size < 10) return;
	VC_Print("WebDevTask_Callback", BlueColor);

	const uint8_t* u_data = static_cast<const uint8_t*>(data);

	switch (u_data[0])
	{
	// Устройство в сети
	case 0x00:
		for (WebDeviceInfo_ListItem& info : WebDevicesList)
		{
			if (info.device.channel == channel)
			{
				info.last_activity = TimeUtils::GetCurrentMs();
				switch (u_data[1])
				{
				case INFO_NAME_0_6:
					memcpy(&info.device.web_name[0], &u_data[2], 6);
					break;
				case INFO_NAME_7_12:
					memcpy(&info.device.web_name[6], &u_data[2], 6);
					break;
				case INFO_ID_ACTIVE_LOBBY:
					memcpy(&info.device.id, &u_data[2], 4);
					memcpy(&info.device.lobby_owner, &u_data[6], 4);
					break;
				case INFO_AVATAR_JOIN_LOBBY:
					memcpy(&info.device.avatar_id, &u_data[2], 4);
					memcpy(&info.device.joining_lobby, &u_data[6], 4);
					break;
				case INFO_FLAGS:
					memcpy(&info.device.flags, &u_data[2], 1);
					break;
				}
				return;
			}
		}
		if (WebDevicesList.Count() < MaxWebDevices)
		{
			WebDeviceInfo_ListItem info{};
			info.device.channel = channel;
			info.last_activity = TimeUtils::GetCurrentMs();
			WebDevicesList.Push(info);
		}
		return;
	// Устройство активно и находится в приложении
	case 0x03:
		for (WebDeviceInfo_ListItem& info : WebDevicesList)
		{
			if (info.device.channel == channel)
			{
				info.device.app_id = ((u_data[2] << 8) | u_data[1]);
				info.last_app_activity = TimeUtils::GetCurrentMs();
			}
		}
		return;
	default:
		return;
	}
}

WebDevicesTask::WebDevicesTask(const StaticText32& name, uint32_t period_ms, uint8_t priority)
	: PriorityTask{ name, period_ms, priority }
{
	refresh_timer.Start(500);

	inited = WebManager_V1::Instance().CreateConnection(&connection, WC_CHANNEL_ALL, WC_PORT_INFO, 0, &WebDevTask_Callback);
}

WebDevicesTask::~WebDevicesTask()
{
	WebManager_V1::Instance().CloseConnection(connection);
}
