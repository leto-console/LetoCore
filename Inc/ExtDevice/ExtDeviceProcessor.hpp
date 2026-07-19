/**
 * @file ExtDeviceProcessor.hpp
 * @date Jul 18, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_EXT_DEVICE_PROCESSOR_HPP_
#define INC_SYSTEM_EXT_DEVICE_PROCESSOR_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <TaskHandler/PriorityTask.hpp>
#include <ExtDevice/ExtDevice.hpp>
#include <Data/StaticList.hpp>

/**
 * Обработчик внешних устройств
 */
class LIBRARIES_EXPORT ExtDeviceProcessor : public PriorityTask
{
public:
    ExtDeviceProcessor(const StaticText32& name, uint32_t period_ms, uint8_t priority);

    void AddExtDevice(ExtDevice* ext_device);
    bool Do() override;
    
    ExtDevice* Find(const char* name) const;

private:
    StaticList<ExtDevice*, 16> ext_devices;
};

#endif
