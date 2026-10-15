#include "LobbyDataProcessor.hpp"

#include <LetoAPI_V1/LetoAPI_V1.h>

void LobbyDataProcessor::Enable(bool enable)
{
    enabled = enable;
}

void LobbyDataProcessor::RegLobbyData(LobbyData *data)
{
    pool.push_back(data);
}

void LobbyDataProcessor::UnregLobbyData(LobbyData *data)
{
    pool.RemoveIf([&](LobbyData* lobby_data) { return lobby_data == data; });
}

void LobbyDataProcessor::ClearLobbyData()
{
    pool.clear();
}

void LobbyDataProcessor::ReceiveData(uint32_t id, const void *data, uint32_t size)
{
    if (!enabled) return;
    
    for (LobbyData* lobby_data : pool)
    {
        lobby_data->ProcessReceive(id, data, size);
    }
}

void LobbyDataProcessor::Loop()
{
    if (!enabled) return;

    uint8_t buffer[16]{};
    for (LobbyData* data : pool)
    {
        data->CheckTimeout();
        if (data->PrepareSend(buffer, sizeof(buffer)))
        {
            leto_api_v1->Lobby->SendData(buffer, sizeof(buffer));
        }
    }
}
