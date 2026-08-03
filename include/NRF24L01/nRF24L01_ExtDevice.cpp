#include "nRF24L01_ExtDevice.hpp"

#ifdef USE_HAL_DRIVER
#include <NRF24L01/nrf24l01.h>
#endif

nRF24L01_ExtDevice::nRF24L01_ExtDevice() : ExtDevice{ "NRF24L01" }
{
#ifdef USE_HAL_DRIVER
    DWT_Init();
#endif
}

void nRF24L01_ExtDevice::Enable()
{
}

void nRF24L01_ExtDevice::Disable()
{
}

bool nRF24L01_ExtDevice::DeviceInit()
{
#ifdef USE_HAL_DRIVER
    uint8_t res = NRF_Init(); // инициализация
    if (!res) return false;

    ////////////// SET ////////////////
    //enableAckPayload();
    setAddressWidth(4); // Я ДОБАВИЛ
    setAutoAck(true);
    //setPayloadSize(3);
    setChannel(32);
    // Delay: 1 (500mks), count: 5
    setRetries(1, 5);

    maskIRQ(true, true, true);
    return true;
#else
    return true;
#endif
}

bool nRF24L01_ExtDevice::DevicePing()
{
#ifdef USE_HAL_DRIVER
    return NRF24_Ping(status == ExtDeviceStatus::READY);
#else
    return true;
#endif
}

void nRF24L01_ExtDevice::DeviceTick()
{
}

// ======================================================================

nRF24L01_ExtDevice* nrf24l01_extdev = nullptr;

// ======================================================================
