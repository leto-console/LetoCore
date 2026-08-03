#include "ST7735_ExtDevice.hpp"

#ifdef USE_HAL_DRIVER
#include <ST7735/low_level/ST7735.h>
#endif

ST7735_ExtDevice::ST7735_ExtDevice() : ExtDevice{ "ST7735" }
{
#ifdef USE_HAL_DRIVER
    ST7735_Unselect();
#endif
}

void ST7735_ExtDevice::Enable()
{
}

void ST7735_ExtDevice::Disable()
{
}

bool ST7735_ExtDevice::DeviceInit()
{
    return true;
}

bool ST7735_ExtDevice::DevicePing()
{
    return true;
}

void ST7735_ExtDevice::DeviceTick()
{
}

// ======================================================================

ST7735_ExtDevice* st7735_extdev = nullptr;

// ======================================================================
