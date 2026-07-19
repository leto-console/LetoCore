#include "SDCard_ExtDevice.hpp"

#ifdef USE_HAL_DRIVER
#include <SDCard/sdcard.h>
#endif

bool SDCard_ExtDevice::DeviceInit()
{
#ifdef USE_HAL_DRIVER
    int init = SDCARD_Init();
    return init == 0;
#else
    return true;
#endif
}

SDCard_ExtDevice::SDCard_ExtDevice() : ExtDevice{ "SD_CARD" }
{
}

void SDCard_ExtDevice::Enable()
{
}

void SDCard_ExtDevice::Disable()
{
}

void SDCard_ExtDevice::DeviceTick()
{
    if (status == ExtDeviceStatus::BAD_INIT)
        return;

#ifdef USE_HAL_DRIVER
    int ping = SDCARD_Ping();
    if (ping == 0)
        status = ExtDeviceStatus::READY;
    else
        status = ExtDeviceStatus::DISCONNECTED;
#endif
}

SDCard_ExtDevice* sdcard_extdev = nullptr;
