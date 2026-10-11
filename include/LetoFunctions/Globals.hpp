#pragma once

#include <LetoFunctions/CheckAPI.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

namespace leto::globals
{
    
    /**
     * @brief Get application dynamic memory allocator
     */
    inline const LetoAllocator_V1* GetAllocator()
    {
        LETO_CHECK_API(leto_api_v1, Globals, GetAllocator, nullptr);
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
        LETO_CHECK_API(leto_api_v1, Globals, GetDebugMode, false);
        return leto_api_v1->Globals->GetDebugMode();
    }

    /**
     * @brief Get number of milliseconds elapsed since MCU startup
     */
    inline uint32_t GetCurrentMs()
    {
        LETO_CHECK_API(leto_api_v1, Globals, GetCurrentMs, 0);
        return leto_api_v1->Globals->GetCurrentMs();
    }

    /**
     * @brief Get device hardware identifier
     */
    inline uint32_t GetDeviceID()
    {
        LETO_CHECK_API(leto_api_v1, Globals, GetDeviceID, 0);
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
        LETO_CHECK_API(leto_api_v1, Globals, CalcCRC16, 0);
        return leto_api_v1->Globals->CalcCRC16(data, length);
    }

    /**
     * @brief Get application execution directory.
     * 
     * Always writes a null-terminated string. Truncates if buffer is too small.
     * 
     * @param[out] buffer Destination buffer.
     * @param[in] length Buffer size in bytes.
     * 
     * @return `true` on success, `false` if truncated or app not running.
     */
    inline bool GetAppDir(char* buffer, uint32_t length)
    {
        LETO_CHECK_API(leto_api_v1, Globals, GetAppDir, false);
        return leto_api_v1->Globals->GetAppDir(buffer, length);
    }

    /**
     * @brief Get current system language.
     */
    inline LetoLanguage_V1 GetSystemLanguage(void)
    {
        LETO_CHECK_API(leto_api_v1, Globals, GetSystemLanguage, LETO_LANG_V1_ENG);
        return leto_api_v1->Globals->GetSystemLanguage();
    }

    /**
     * @brief Get application ID.
     * 
     * @return Application ID, or `0` if no app is running.
     */
    inline uint16_t GetAppID(void)
    {
        LETO_CHECK_API(leto_api_v1, Globals, GetAppID, 0);
        return leto_api_v1->Globals->GetAppID();
    }

}