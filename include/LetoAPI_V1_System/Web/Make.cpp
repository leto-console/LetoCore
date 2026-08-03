#include "Make.hpp"

#include <cstdio>
#include <cstdarg>

// ====================================================================================================

#include <LetoAPI_V1_System/Web/WebManager.hpp>

static bool CreateConnection (WebConnection_V1* connection, uint8_t channel, uint8_t port, uint32_t id, WebConnection_V1_Callback callback)
{
    return WebManager_V1::Instance().CreateConnection(connection, channel, port, id, callback);
}

static void CloseConnection (WebConnection_V1 connection)
{
    WebManager_V1::Instance().CloseConnection(connection);
}

static void  SendData (WebConnection_V1 connection, const void* data, uint32_t size)
{
    WebManager_V1::Instance().SendData(connection, data, size);
}

static uint32_t GetDevicesNear (WebDeviceInfo_V1* info, uint32_t available)
{
    return WebManager_V1::Instance().GetDevicesNear(info, available);
}

static bool FindDeviceNear(uint32_t id, WebDeviceInfo_V1* info)
{
    return WebManager_V1::Instance().FindDeviceNear(id, info);
}

// ====================================================================================================

const WebAPI_V1* Make_WebAPI()
{
    static const WebAPI_V1 api
    {
        &CreateConnection,
        &CloseConnection,
        &SendData,
        &GetDevicesNear,
        &FindDeviceNear
    };
	
    return &api;
}
