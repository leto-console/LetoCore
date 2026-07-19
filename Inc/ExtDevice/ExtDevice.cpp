#include "ExtDevice.hpp"

ExtDevice::ExtDevice(const char * device_name) 
    : device_name{ device_name }
{
}

const char *ExtDevice::GetName() const
{
    return device_name;
}

void ExtDevice::AsyncInit(uint8_t retries)
{
    status = ExtDeviceStatus::INITIALIZING;
    init_retries = retries;
    init_allow = false;
}

void ExtDevice::Init(uint8_t retries)
{
    status = ExtDeviceStatus::INITIALIZING;
    init_retries = retries;
    init_allow = true;
}

void ExtDevice::MainLoop()
{
    if (init_retries && init_allow)
    {
        init_retries--;
        if (DeviceInit())
            init_retries = 0;
        else if (!init_retries)
            status = ExtDeviceStatus::BAD_INIT;
        return;
    }
    DeviceTick();
}

ExtDeviceStatus ExtDevice::GetStatus(bool draw)
{
    if (init_retries && draw) init_allow = true;
    return status;
}
