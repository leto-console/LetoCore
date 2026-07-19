#include "ExtDeviceProcessor.hpp"

#include <cstring>

ExtDeviceProcessor::ExtDeviceProcessor(const StaticText32 &name, uint32_t period_ms, uint8_t priority)
    : PriorityTask{ name, period_ms, priority }
{
}

void ExtDeviceProcessor::AddExtDevice(ExtDevice *ext_device)
{
    ext_devices.push_back(ext_device);
}

bool ExtDeviceProcessor::Do()
{
    // Add CPU time balancer (if the device takes longer than, for example, 15 ms to update, we add it to the blacklist)
    for (ExtDevice* device : ext_devices)
        device->MainLoop();
    return true;
}

ExtDevice* ExtDeviceProcessor::Find(const char * name) const
{
    for (ExtDevice* device : ext_devices)
    {
        if (strcmp(device->GetName(), name))
            return device;
    }

    return nullptr;
}
