/**
 * @file GlobalsAPI_V1.hpp
 * @date May 23, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_GLOBALS_GLOBALS_API_V1_HPP_
#define INC_LETO_API_V1_GLOBALS_GLOBALS_API_V1_HPP_

#include <cstdint>
#include <type_traits>

#include <Data/IAllocator.hpp>

// Align to 4-byte boundary
#pragma pack(push, 4)

/// Global context functions
struct GlobalsAPI_V1
{
    // ===================================================
    //               Dynamic Memory Management            
    // ===================================================

    /**
     * @brief Allocate memory in application dynamic memory
     * 
     * @param size Size of allocated memory in bytes
     * @return Pointer to the allocated memory block
     */
    void* (*Alloc)(uint32_t size);

    /**
     * @brief Free memory in application dynamic memory
     * 
     * @param ptr Pointer to the memory block
     */
    void (*Free)(const void* ptr);

    /// TODO: Warning! Potential single point of failure.
    /// The allocator can crash the entire application.
    /**
     * @brief Get application dynamic memory allocator
     */
    IAllocator* (*GetAllocator)();

    // ===================================================
    //                   State Management                 
    // ===================================================

    /**
     * @brief Get system debug mode state
     * @return `true` if enabled, `false` if disabled
     */
    bool (*GetDebugMode)();

    /**
     * @brief Get number of milliseconds elapsed since MCU startup
     */
    uint32_t (*GetCurrentMs)();

    /**
     * @brief Get device hardware identifier
     */
    uint32_t (*GetDeviceID)();
};

#pragma pack(pop)

static_assert(std::is_standard_layout<GlobalsAPI_V1>::value, "GlobalsAPI_V1 must be a standard layout type");

#endif
