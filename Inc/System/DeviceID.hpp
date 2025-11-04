/**
 * @file DeviceID.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_DEVICE_ID_HPP_
#define INC_SYSTEM_DEVICE_ID_HPP_

#include "LibrariesExport.h"

#include <cstdint>

/**
 * @brief Установить идентификатор устройства
 */
extern LIBRARIES_EXPORT void SetDeviceID(uint32_t id);

/**
 * @brief Получить идентификатор устройства
 */
extern LIBRARIES_EXPORT uint32_t GetDeviceID();

#endif
