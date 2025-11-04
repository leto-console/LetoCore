#include "LobbyData.hpp"

LobbyData::LobbyData(uint8_t data_id, uint32_t period_ms)
    : data_id { data_id }, period_ms{ period_ms }
{
    send_timer.Start(period_ms);
}

bool LobbyData::PrepareSend(void *data, uint32_t size)
{
    if (size < 3 || !CheckTimer()) 
        return false;

    uint8_t* u_data = static_cast<uint8_t*>(data);
    u_data[0] = data_id;

    if (!OnSend(&u_data[2], size - 2)) 
        return false;
    
    RestartTimer();
    return true;
}

void LobbyData::ProcessReceive(uint32_t id, const void *data, uint32_t size)
{
    if (size < 3) return;

    const uint8_t* u_data = static_cast<const uint8_t*>(data);
    if (u_data[0] != data_id) return;

    OnReceive(id, &u_data[2], size - 2);
}
