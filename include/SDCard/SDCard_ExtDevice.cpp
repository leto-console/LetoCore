#include "SDCard_ExtDevice.hpp"

#include <VirtualConsole/VirtualConsole.hpp>

#ifdef USE_HAL_DRIVER
#include <SDCard/sdcard.h>
#endif

#include <Time/TimeUtils.hpp>

bool SDCard_ExtDevice::DeviceInit()
{
#ifdef USE_HAL_DRIVER
    SDCARD_SetSpeed(SPI_BAUDRATEPRESCALER_256);
    int code = SDCARD_Init();
    SDCARD_SetSpeed(SPI_BAUDRATEPRESCALER_2);

    bool success = (code == 0);

    VC_Printf("SDCARD_Init() %s", success ? GreenColor : RedColor, "success", "failed");
    VC_Printf("code = %d\r\n", success ? GreenColor : RedColor, code);
    VC_Printf("R1 = %d\r\n", BlueColor, SDCARD_LastReadR1());

    return success;
#else
    TimeUtils::SleepMs(10);
    return true;
#endif
}

bool SDCard_ExtDevice::DevicePing()
{
#ifdef USE_HAL_DRIVER
    int ping = SDCARD_Ping(status == ExtDeviceStatus::READY);
    return ping == 0;
#else
    TimeUtils::SleepMs(1);
    return true;
#endif
}

SDCard_ExtDevice::SDCard_ExtDevice() : ExtDevice{ "SD_CARD" }
{
#ifdef USE_HAL_DRIVER
    SDCARD_Unselect();
#endif
}

void SDCard_ExtDevice::Enable()
{
}

void SDCard_ExtDevice::Disable()
{
}

void SDCard_ExtDevice::DeviceTick()
{
}

// ======================================================================

SDCard_ExtDevice* sdcard_extdev = nullptr;

// ======================================================================
