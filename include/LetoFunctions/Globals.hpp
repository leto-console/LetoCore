#pragma once

#include <LetoAPI_V1/LetoAPI_V1.h>

namespace leto::globals
{
    
    /**
     * @brief Get application dynamic memory allocator
     */
    inline const LetoAllocator_V1* GetAllocator()
    {
        return leto_api_v1->Globals->GetAllocator();
    }

    // ===================================================
    //                   State Management                 
    // ===================================================

    /**
     * @brief Get system debug mode state
     * @return `true` if enabled, `false` if disabled
     */
    inline bool GetDebugMode()
    {
        return leto_api_v1->Globals->GetDebugMode();
    }

    /**
     * @brief Get number of milliseconds elapsed since MCU startup
     */
    inline uint32_t GetCurrentMs()
    {
        return leto_api_v1->Globals->GetCurrentMs();
    }

    /**
     * @brief Get device hardware identifier
     */
    inline uint32_t GetDeviceID()
    {
        return leto_api_v1->Globals->GetDeviceID();
    }

    /**
     * @brief Calculates the CRC16 checksum for a given data buffer.
     * 
     * @param[in] data   Pointer to the input data buffer.
     * @param[in] length Size of the data buffer in bytes.
     * 
     * @return The calculated 16-bit CRC value.
     */
    inline uint16_t CalcCRC16(const void* data, uint32_t length)
    {
        return leto_api_v1->Globals->CalcCRC16(data, length);
    }

}