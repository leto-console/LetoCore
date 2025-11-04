#include "WebManager.hpp"

#include <Time/TimeUtils.hpp>
#include <LetoAPI_V1_System/Web/WebDevicesList.hpp>

#include <System/DeviceID.hpp>

bool WebManager_V1::FindConnection(WebConnection_PoolItem** connection, uint8_t channel, uint8_t port, uint32_t id)
{
    for (WebConnection_PoolItem& item : pool)
    {
        if (item.connection.port == port &&
            (!item.connection.channel || item.connection.channel == channel)&&
            (!item.connection.id || item.connection.id == id))
        {
            item.last_active_ms = TimeUtils::GetCurrentMs();
            if (connection)
                *connection = &item;
            return true;
        }
    }
    return false;
}

bool WebManager_V1::CreateConnection(WebConnection_V1 *connection, uint8_t channel, uint8_t port, uint32_t id, WebConnection_V1_Callback callback)
{
    if (!connection || pool.Full()) return false;

    *connection = WebConnection_V1{ channel, port, id, callback };
    WebConnection_PoolItem item{ *connection, TimeUtils::GetCurrentMs() };
    item.last_active_ms = TimeUtils::GetCurrentMs();

    pool.push_back(item);
    return true;
}

void WebManager_V1::CloseConnection(WebConnection_V1 connection)
{
    for (WebConnection_PoolItem& item : pool)
    {
        if (memcmp(&item.connection, &connection, sizeof(WebConnection_V1)) == 0)
        {
            pool.erase(&item);
            return;
        }
    }
}

void WebManager_V1::SendData(WebConnection_V1 connection, const void *data, uint32_t size)
{
    if (!FindConnection(nullptr, connection.channel, connection.port, connection.id))
        return;

    WebPacket_BufferItem item{};
    item.channel = connection.channel;

    item.packet.id_from = GetDeviceID();
    item.packet.id_to = connection.id;
    item.packet.port = connection.port;
    memcpy(item.packet.data, data, size > sizeof(item.packet.data) ? sizeof(item.packet.data) : size);

    buffer.Push(item);
    buffer_cnt++;
}

uint32_t WebManager_V1::GetDevicesNear(WebDeviceInfo_V1 *info, uint32_t available) const
{
    uint32_t count = 0;
    for (WebDeviceInfo_ListItem dev_info : WebDevicesList)
    {
        info[count] = dev_info.device;
        if (++count >= available) break;
    }  
    return count;
}

bool WebManager_V1::FindDeviceNear(uint32_t id, WebDeviceInfo_V1 *info) const
{
    for (WebDeviceInfo_ListItem dev_info : WebDevicesList)
    {
        if (dev_info.device.id != id) continue;
        if (info) *info = dev_info.device;
        return true;
    }  

    return false;
}

uint32_t WebManager_V1::GetConnections(WebConnection_V1 *info, uint32_t available)
{
    uint32_t cnt = 0;
    for (WebConnection_PoolItem& item : pool)
    {
        if (info && available)
        {
            info[cnt] = item.connection;
            if (cnt + 1 >= available) break;
        }
        cnt++;
    }
    return cnt;
}

void WebManager_V1::SendSynchronizeData(WebConnection_V1 connection, const void *data, uint32_t size)
{
    WebConnection_PoolItem* pool_item{};
    if (!FindConnection(&pool_item, connection.channel, connection.port, connection.id))
        return;

    WebPacket_BufferItem item{};
    item.channel = connection.channel;

    item.packet.id_from = GetDeviceID();
    item.packet.id_to = connection.id;
    item.packet.port = connection.port;
    item.packet.flags |= WP_FLAG_SYN;
    memcpy(item.packet.data, data, size > sizeof(item.packet.data) ? sizeof(item.packet.data) : size);

    // Запрет на отправление одинаковых данных при активной синхронизации
    for (WebPacket_SyncItem& sync_item : sync_out)
    {
        if (sync_item.buffer_item.channel == item.channel &&
            sync_item.buffer_item.packet == item.packet)
            return;
    }

    WebPacket_SyncItem sync_info{};
    sync_info.buffer_item = item;
    
    sync_out.Push(sync_info);
}

void WebManager_V1::ReceiveData(uint8_t channel, const WebPacket_V1& packet)
{
    /**
     * Если получено подтверждение отправляемого сообщения, сбрасываем его
     */
    if (packet.flags & WP_FLAG_ACK)
    {
        for (WebPacket_SyncItem& item : sync_out)
        {
            if (item.buffer_item.channel == channel &&
                item.buffer_item.packet == packet &&
                item.buffer_item.packet.retries == packet.retries)
            {
                sync_out.Erase(&item);
                break;
            }
        }

        return;
    }

    // Обрабатываются либо широковещательные пакеты, либо с нужными ID
    if (packet.id_to && packet.id_to != GetDeviceID())
        return;

    // Свои пакеты не обрабатываются
    if (packet.id_from == GetDeviceID())
        return;

    WebConnection_PoolItem* pool_item{};
    if (!FindConnection(&pool_item, channel, packet.port, packet.id_from) || !pool_item->connection.callback)
        return;

    if (packet.flags & WP_FLAG_SYN)
    {
        for (WebPacket_SyncItem& item : sync_in)
        {
            if (item.buffer_item.channel == channel &&
                item.buffer_item.packet == packet)
            {
                if (item.buffer_item.packet.retries < packet.retries)
                {
                    item.buffer_item.packet = packet;
                    item.buffer_item.packet.flags &=~ (WP_FLAG_ACK);
                    item.buffer_item.packet.flags &=~ (WP_FLAG_SYN);
                    item.buffer_item.packet.flags |= WP_FLAG_ACK;

                    item.last_send_ms = TimeUtils::GetCurrentMs();

                    buffer.Push(item.buffer_item);
                    buffer_cnt++;
                }
                return;
            }
        }

        WebPacket_SyncItem item{};
        item.buffer_item.channel = channel;
        item.buffer_item.packet = packet;

        item.buffer_item.packet.flags &=~ (WP_FLAG_ACK);
        item.buffer_item.packet.flags &=~ (WP_FLAG_SYN);
        item.buffer_item.packet.flags |= WP_FLAG_ACK;

        item.last_send_ms = TimeUtils::GetCurrentMs();

        sync_in.Push(item);
        buffer.Push(item.buffer_item);
        buffer_cnt++;
    }

    pool_item->connection.callback(channel, packet.port, packet.id_from, packet.data, sizeof(packet.data));
}

bool WebManager_V1::CheckBuffer(WebPacket_BufferItem &info)
{
    return buffer.Front(info);
}

void WebManager_V1::NextBuffer()
{
    buffer.Pop();
}

void WebManager_V1::Loop()
{
    const uint32_t TIMEOUT_MS = 200;
    const uint8_t MAX_RETRIES = 10;

    for (WebPacket_SyncItem& item : sync_out.reversed())
    {
        if (item.buffer_item.packet.retries >= MAX_RETRIES)
        {
            sync_out.Erase(&item);
            continue;
        }
        if (TimeUtils::GetCurrentMs() - item.last_send_ms >= TIMEOUT_MS)
        {
            item.buffer_item.packet.retries += 1;
            buffer.Push(item.buffer_item);
            buffer_cnt++;
            item.last_send_ms = TimeUtils::GetCurrentMs();
        }
    }

    const uint32_t TIMEOUT_IN_MS = 400;

    for (WebPacket_SyncItem& item : sync_in.reversed())
    {
        if (TimeUtils::GetCurrentMs() - item.last_send_ms >= TIMEOUT_IN_MS)
            sync_in.Erase(&item);
    }

    
    if (buffer_sec_timer.Expired())
    {
        buffer_sec_timer.Start();
        buffer_pps = buffer_cnt;
        buffer_cnt = 0;
    }
}
