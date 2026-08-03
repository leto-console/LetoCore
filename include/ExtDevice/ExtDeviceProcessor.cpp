#include "ExtDeviceProcessor.hpp"

#include <cstring>

ExtDeviceProcessor::ExtDeviceProcessor(const StaticText32 &name, uint32_t period_ms, uint8_t priority)
    : PriorityTask{ name, period_ms, priority }
{
}

void ExtDeviceProcessor::AddExtDevice(ExtDevice* ext_device)
{
    ext_devices.push_back(ext_device);
}

bool ExtDeviceProcessor::Do()
{
    /// TODO: Add CPU time balancer (if the device takes longer than, for example, 15 ms to update, we add it to the blacklist)
    if (ext_devices.Count() == 0) return true;
    if (proc_idx >= ext_devices.Count())
    {
        proc_idx = 0;
        return true;
    }

    // One device is processed per cycle
    ExtDevice* device = ext_devices[proc_idx];

    if (device->GetStatus() == ExtDeviceStatus::CONNECTED)
        device->Init();

    device->MainLoop();

    if (++proc_idx == ext_devices.Count())
    {
        proc_idx = 0;
        return true;
    }

    return false;
}

ExtDevice* ExtDeviceProcessor::Find(const char* name) const
{
    for (ExtDevice* device : ext_devices)
    {
        if (strcmp(device->GetName(), name))
            return device;
    }

    return nullptr;
}
