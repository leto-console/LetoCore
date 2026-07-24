/**
 * @file nRF24L01_ExtDevice.hpp
 * @date Jul 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_NRF24L01_NRF24L01_EXT_DEVICE_HPP_
#define INC_NRF24L01_NRF24L01_EXT_DEVICE_HPP_

#include "LibrariesExport.h"

#include <ExtDevice/ExtDevice.hpp>

struct LIBRARIES_EXPORT nRF24L01_ExtDevice : public ExtDevice
{
public:
    nRF24L01_ExtDevice();

    ExtDeviceFeatures GetFeatures() override { return ExtDeviceFeatures::NONE; }

    void Enable() override;

    void Disable() override;

protected:
    bool DeviceInit() override;

    bool DevicePing() override;

    void DeviceTick() override;
};

extern LIBRARIES_EXPORT nRF24L01_ExtDevice* nrf24l01_extdev;

#endif
