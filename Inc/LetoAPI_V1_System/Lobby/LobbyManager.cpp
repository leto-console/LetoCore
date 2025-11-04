#include "LobbyManager.hpp"

#include "LobbyInfoPacket.hpp"

#include <LetoAPI_V1_System/Web/WebManager.hpp>
#include <LetoAPI_V1_System/Web/WebDevicesList.hpp>

#include <Time/TimeUtils.hpp>

#include <System/DeviceID.hpp>

#include "LobbyCallbacks.hpp"


LobbyManager_V1::LobbyManager_V1()
{
    find_timer.Start(100);
    WebManager_V1::Instance().CreateConnection(&find_connection, WC_CHANNEL_ALL, WC_PORT_INF_LOBBY, 0, &LobbyInfoCallback);
    WebManager_V1::Instance().CreateConnection(&main_conection, WC_CHANNEL_ALL, WC_PORT_CON_LOBBY, 0, &LobbyMainCallback);
}

LobbyManager_V1::~LobbyManager_V1()
{
    WebManager_V1::Instance().CloseConnection(find_connection);
    WebManager_V1::Instance().CloseConnection(main_conection);
}

bool LobbyManager_V1::IsMember(uint32_t id, uint32_t* pos) const
{
    if (global_lobby.state == LC_STATE_DISCONNECTED) return false;

    for (int i = 0; i < global_lobby.connected; ++i)
    {
        if (global_lobby.members[i] == id)
        {
            if (pos) *pos = i;
            return true;
        }
    }
    return false;
}

void LobbyManager_V1::SendCommonInfo() const
{
    static LobbyInfoPacket main_info{ FLP_INFO };
    main_info.info.owner = global_lobby.owner;
    main_info.info.connected = global_lobby.connected;
    main_info.info.max_count = global_lobby.max_count;
    main_info.info.app_id = global_lobby.app_id;
    
    WebManager_V1::Instance().SendData(find_connection, &main_info, sizeof(LobbyInfoPacket));
}

void LobbyManager_V1::SendMembersInfo(uint32_t part) const
{
    static LobbyInfoPacket user_info{ FLP_MEMBERS_INFO };

    switch (part)
    {
    case 0:
    {
        user_info.user_info.number = 0;
        memcpy(user_info.user_info.members, &global_lobby.members[0], 3 * sizeof(uint32_t));
        
        WebManager_V1::Instance().SendData(find_connection, &user_info, sizeof(LobbyInfoPacket));
        break;
    }
    case 1:
    {
        user_info.user_info.number = 1;
        memcpy(user_info.user_info.members, &global_lobby.members[3], 3 * sizeof(uint32_t));
        
        WebManager_V1::Instance().SendData(find_connection, &user_info, sizeof(LobbyInfoPacket));
        break;
    }
    default:
        break;
    }
}

void LobbyManager_V1::GlobalReset()
{
    lobbies_near.clear();
    memset(&global_lobby, 0, sizeof(LobbyConnection_V1));
}

uint32_t LobbyManager_V1::GetLobbiesNear(LobbyInfo_V1 *info, uint32_t available)
{
    uint32_t count = 0;
    for (LobbyInfo_V1& lobby : lobbies_near)
    {
        info[count] = lobby;
        if (++count >= available) break;
    }  
    return count;
}

bool LobbyManager_V1::CreateLobby(LobbyConnection_V1 *lobby, uint16_t app_id, uint8_t max_count, LobbyConnection_V1_Callback callback)
{
    if (global_lobby.state != LC_STATE_DISCONNECTED) return false;

    memset(&global_lobby, 0, sizeof(LobbyConnection_V1));

    global_lobby.owner = global_lobby.members[0] = GetDeviceID();
    global_lobby.app_id = app_id;
    global_lobby.connected = 1;
    global_lobby.max_count = max_count;
    global_lobby.callback = callback;
    global_lobby.state = LC_STATE_CONNECTED;

    if (lobby) *lobby = global_lobby;

    return true;
}

void LobbyManager_V1::JoinLobby(LobbyInfo_V1 info, LobbyConnection_V1_Callback callback)
{
    if (global_lobby.state != LC_STATE_DISCONNECTED) return;

    memset(&global_lobby, 0, sizeof(LobbyConnection_V1));
    global_lobby.owner = info.owner;
    global_lobby.app_id = info.app_id;
    global_lobby.connected = info.connected;
    global_lobby.max_count = info.max_count;
    global_lobby.callback = callback;
    global_lobby.state = LC_STATE_JOINING;

    // С момента запроса не анализируется состояние онлайна хоста
    join_timer.Start(3000);
}

bool LobbyManager_V1::GetActiveLobby(LobbyConnection_V1 *lobby)
{
    if (global_lobby.state != LC_STATE_CONNECTED) return false;
    if (lobby) *lobby = global_lobby;
    return true;
}

bool LobbyManager_V1::GetJoiningLobby(LobbyConnection_V1 *lobby)
{
    if (global_lobby.state != LC_STATE_JOINING) return false;
    if (lobby) *lobby = global_lobby;
    return true;
}

void LobbyManager_V1::DisconnectMember(uint32_t ID)
{
    if (global_lobby.state != LC_STATE_CONNECTED || global_lobby.owner != GetDeviceID())
        return;
    
    for (uint8_t i = 0; i < global_lobby.connected && i < LC_MEMBERS_COUNT; ++i)
    {
        if (global_lobby.members[i] == ID)
        {
            global_lobby.members[i] = 0;
            if (i < global_lobby.connected)
                memmove(&global_lobby.members[i], &global_lobby.members[i + 1], (global_lobby.connected - i - 1) * sizeof(uint32_t));
            global_lobby.connected--;
            break;
        }
    }
}

void LobbyManager_V1::QuitLobby()
{    
    global_lobby.state = LC_STATE_DISCONNECTED;
}

void LobbyManager_V1::SendData(const void *data, uint32_t size)
{
    if (global_lobby.state != LC_STATE_CONNECTED)
        return;
    
    WebManager_V1::Instance().SendData(main_conection, data, size);
}

void LobbyManager_V1::Loop()
{
    // Раз в 100 мс меняется счетчик
    static uint8_t cnt = 0xFF;

    if (!find_timer.Expired())
        return;
    
    cnt++;
    find_timer.Start();

    // Раз в 600 мс (2-ой цикл)
    if (cnt % 6 == 2)
    {
        const uint32_t LOBBY_INFO_TIMEOUT = 4000;
        for (LobbyInfo_V1& info : lobbies_near.reversed())
        {
            if (TimeUtils::GetCurrentMs() - info.last_info > LOBBY_INFO_TIMEOUT)
                lobbies_near.erase(&info);
        }
    }

    if (global_lobby.state == LC_STATE_DISCONNECTED)
        return;

    ///< Действия выполняемые создателем лобби
    if (global_lobby.owner == GetDeviceID())
    {
        // Раз в 200 мс (1-ый цикл)
        if (cnt % 2 == 1)
        {
            bool was_added = false;
            
            for (WebDeviceInfo_ListItem& info : WebDevicesList)
            {
                if (info.device.joining_lobby && 
                    info.device.joining_lobby == GetDeviceID() &&
                    global_lobby.connected < global_lobby.max_count)
                {
                    // Устройство уже подключено к лобби
                    if (IsMember(info.device.id))
                        return;

                    global_lobby.members[global_lobby.connected++] = info.device.id;

                    // 5 секунд с момента подключения не анализируется активность абонентов
                    join_timer.Start(5000);
                    was_added = true;
                }
            }

            // Отправить вне очереди информацию о подключенных устройствах
            if (was_added)
            {
                SendCommonInfo();
                SendMembersInfo(0);
                SendMembersInfo(1);
            }
        }

        // Раз в 700 мс (6-ой цикл)
        if (cnt % 7 == 6 && join_timer.Expired(false))
        {
            bool member_active[LC_MEMBERS_COUNT]{};
            member_active[0] = true;    ///< Хост всегда активен

            // Контроль активности участников
            for (WebDeviceInfo_ListItem& info : WebDevicesList)
            {
                uint32_t pos;
                if (!IsMember(info.device.id, &pos)) continue;

                member_active[pos] = (info.device.lobby_owner == GetDeviceID());
            }
            
            for (int i = global_lobby.connected - 1; i != 0; --i)
            {
                if (!member_active[i])
                    DisconnectMember(global_lobby.members[i]);
            }
        }
        
        // Раз в 700 мс (0-ый цикл)
        if (cnt % 7 == 0)
        {
            SendCommonInfo();
        }

        // Раз в 700 мс (3-ий цикл)
        if (cnt % 7 == 3)
        {
            SendMembersInfo(0);
        }

        // Раз в 700 мс (5-ый цикл)
        if (cnt % 7 == 5)
        {
            SendMembersInfo(1);
        }
    }
    ///< Действия выполняемые участниками лобби
    else
    {
        // Раз в 900 мс (7-ой цикл)
        if (cnt % 9 == 7 && join_timer.Expired(false))
        {
            bool host_active{ false };

            // Контроль активности хоста лобби
            for (const LobbyInfo_V1& info : lobbies_near)
            {
                if (info.owner == global_lobby.owner)
                {
                    host_active = true;
                    break;
                }
            }

            if (!host_active)
            {
                global_lobby.state = LC_STATE_DISCONNECTED;
                /// TODO: Возможно какой-то специфический колбек о том, что соединение разорвано
                return;
            }

            // Контролю подключения к хосту
            if (!IsMember(GetDeviceID()))
            {
                global_lobby.state = LC_STATE_DISCONNECTED;
                /// TODO: Возможно какой-то специфический колбек о том, что соединение разорвано
                return;
            }
            else if (global_lobby.state == LC_STATE_JOINING)
            {
                global_lobby.state = LC_STATE_CONNECTED;
            }
        }
    }
}
