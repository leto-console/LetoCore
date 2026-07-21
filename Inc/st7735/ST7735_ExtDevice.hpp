/**
 * @file ST7735_ExtDevice.hpp
 * @date Jul 21, 2026
 * @author Rakhimov T.
 */

#ifndef INC_ST7735_ST7735_EXT_DEVICE_HPP_
#define INC_ST7735_ST7735_EXT_DEVICE_HPP_

#include "LibrariesExport.h"

#include <ExtDevice/ExtDevice.hpp>

struct LIBRARIES_EXPORT ST7735_ExtDevice : public ExtDevice
{
public:
    ST7735_ExtDevice();

    ExtDeviceFeatures GetFeatures() override { return ExtDeviceFeatures::NONE; }

    void Enable() override;

    void Disable() override;

protected:
    bool DeviceInit() override;

    bool DevicePing() override;

    void DeviceTick() override;
};

extern LIBRARIES_EXPORT ST7735_ExtDevice* st7735_extdev;

#endif
