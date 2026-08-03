#include "DeviceID.hpp"

#include <Auth/AuthHandler.hpp>

static uint32_t device_id = 0;

void SetDeviceID(uint32_t id)
{
    // Установить системный идентификатор можно только один раз - инициализатору системы
    static bool called = false;
    if (called) return;
    called = true;

    device_id = id;
}

uint32_t GetDeviceID()
{
    return device_id;
}
