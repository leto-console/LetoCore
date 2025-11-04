#include "LobbyCallbacks.hpp"

#include <LetoAPI_V1/Lobby/LobbyConnection.hpp>
#include <LetoAPI_V1/Lobby/LobbyInfo.hpp>

#include <LetoAPI_V1_System/Lobby/LobbyInfoPacket.hpp>
#include <LetoAPI_V1_System/Lobby/LobbyGlobals.hpp>

#include <LetoAPI_V1_System/Web/WebManager.hpp>

#include <Time/TimeUtils.hpp>

#include <System/DeviceID.hpp>

void LobbyInfoCallback(uint8_t /*channel*/, uint8_t /*port*/, uint32_t id, const void* data, uint32_t size)
{
    if (size < sizeof(LobbyInfoPacket)) return;

    LobbyInfoPacket packet = *static_cast<const LobbyInfoPacket*>(data);

    if (global_lobby.state != LC_STATE_DISCONNECTED &&
        (global_lobby.owner != id ||
        global_lobby.owner == GetDeviceID()))
        return;    
    
    if (packet.type == FLP_INFO)
    {
        bool exists = false;
        for (LobbyInfo_V1& info : lobbies_near)
        {
            if (info.owner == packet.info.owner)
            {
                info.last_info = TimeUtils::GetCurrentMs();
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            packet.info.last_info = TimeUtils::GetCurrentMs();
            lobbies_near.push_back(packet.info);
        }

        global_lobby.app_id = packet.info.app_id;
        global_lobby.connected = packet.info.connected;
        global_lobby.max_count = packet.info.max_count;
        return;
    }
    
    if (packet.type == FLP_MEMBERS_INFO)
    {
        // Консистентное получение данных об участниках
        static uint32_t members[6]{};

        if (packet.user_info.number == 0)
        {
            memcpy(&members[0], &packet.user_info.members[0], 3 * sizeof(uint32_t));
        }
        else if (packet.user_info.number == 1)
        {
            memcpy(&members[3], &packet.user_info.members[0], 3 * sizeof(uint32_t));
            memcpy(&global_lobby.members[0], &members[0], 6 * sizeof(uint32_t));

            bool connected = false;
            for (int i = 0; i < global_lobby.connected; ++i)
            {
                if (global_lobby.members[i] == GetDeviceID())
                {
                    connected = true;
                    break;
                }
            }
            if (connected && global_lobby.state == LC_STATE_JOINING)
                global_lobby.state = LC_STATE_CONNECTED;
        }
        
        return;
    }

}

void LobbyMainCallback(uint8_t /*channel*/, uint8_t /*port*/, uint32_t id, const void *data, uint32_t size)
{
    if (global_lobby.state != LC_STATE_CONNECTED || !global_lobby.callback) return;
    
    bool is_member = false;
    for (uint32_t i = 0; i < global_lobby.connected; ++i)
    {
        if (global_lobby.members[i] == id)
        {
            is_member = true;
            break;
        }
    }
    if (!is_member)
        return;

    global_lobby.callback(id, data, size);
}
