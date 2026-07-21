#include "ExtDevice.hpp"

#include <Time/Stopwatch.hpp>

ExtDevice::ExtDevice(const char * device_name) 
    : device_name{ device_name }
{
}

const char* ExtDevice::GetName() const
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
        Stopwatch sw_init(true);
        if (DeviceInit())
        {
            init_retries = 0;
            status = ExtDeviceStatus::READY;
        }
        else if (!init_retries)
            status = ExtDeviceStatus::BAD_INIT;
        time_init = sw_init.ElapsedMks();
        return;
    }
    Stopwatch sw_ping(true);
    bool ping = DevicePing();
    time_ping.Add(sw_ping.ElapsedMks());

    if (ping)
    {
        switch (status)
        {
        case ExtDeviceStatus::UNDEFINED:
        case ExtDeviceStatus::DISCONNECTED:
            status = ExtDeviceStatus::CONNECTED;
            break;
        }
    }
    else
    {
        status = ExtDeviceStatus::DISCONNECTED;
    }

    Stopwatch sw_tick(true);
    DeviceTick();
    time_tick.Add(sw_tick.ElapsedMks());
}

ExtDeviceStatus ExtDevice::GetStatus(bool draw)
{
    if (init_retries && draw) init_allow = true;
    return status;
}

// ======================================================================

StaticList<ExtDevice*, 32> ExtDevices;

// ======================================================================
