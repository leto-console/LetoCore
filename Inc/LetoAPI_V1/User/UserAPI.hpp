/**
 * @file UserAPI_V1.hpp
 * @date May 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_USER_USER_API_V1_HPP_
#define INC_LETO_API_V1_USER_USER_API_V1_HPP_

#include <cstdint>
#include <type_traits>

#include <Storage/NamedSpace.hpp>
#include <Graphics/BitmapData.hpp>

// Align to 4-byte boundary
#pragma pack(push, 4)

/// User management functions
struct UserAPI_V1
{
    /// @brief Get user space
    NamedSpace* (*const GetUserSpace)();
    
    /// @brief Get avatar by identifier
    bool (*const GetAvatarByID)(uint32_t ID, BitmapData* data);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<UserAPI_V1>::value, "UserAPI_V1 must be a standard layout type");

#endif
