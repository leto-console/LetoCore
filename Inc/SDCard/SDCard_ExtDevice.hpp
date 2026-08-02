/**
 * @file SDCard_ExtDevice.hpp
 * @date Jul 15, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SDCARD_SDCARD_EXT_DEVICE_HPP_
#define INC_SDCARD_SDCARD_EXT_DEVICE_HPP_

#include "LibrariesExport.h"

#include <ExtDevice/ExtDevice.hpp>

struct LETO_CORE_EXPORT SDCard_ExtDevice : public ExtDevice
{
public:
    SDCard_ExtDevice();

    ExtDeviceFeatures GetFeatures() override { return ExtDeviceFeatures::NONE; }

    void Enable() override;

    void Disable() override;

protected:
    bool DeviceInit() override;

    bool DevicePing() override;

    void DeviceTick() override;
};

extern LETO_CORE_EXPORT SDCard_ExtDevice* sdcard_extdev;

#endif
