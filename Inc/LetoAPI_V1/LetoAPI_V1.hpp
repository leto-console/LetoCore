/**
 * @file LetoAPI_V1.hpp
 * @date May 22, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LETO_API_V1_HPP_
#define INC_LETO_API_V1_LETO_API_V1_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <type_traits>

#include <LetoAPI_V1/Text/TextAPI.hpp>
#include <LetoAPI_V1/Globals/GlobalsAPI.hpp>
#include <LetoAPI_V1/Web/WebAPI.hpp>
#include <LetoAPI_V1/User/UserAPI.hpp>
#include <LetoAPI_V1/Font/FontAPI.hpp>
#include <LetoAPI_V1/Math/MathAPI.hpp>
#include <LetoAPI_V1/Lobby/LobbyAPI.hpp>

#define LETO_API_V1_MINOR 0

// Align to 4-byte boundary
#pragma pack(push, 4)

struct LetoAPI_V1
{
    /// API version details
    const struct VersionAPI
    {
        uint16_t major;		///< Major API version
        uint16_t minor;		///< Minor API version
    }
    *const Version;

    /// Text processing functions
    const TextAPI_V1* const Text;
    
    /// Global context functions
    const GlobalsAPI_V1* const Globals;

    /// Network interaction functions
    const WebAPI_V1* const Web;

    /// User management functions
    const UserAPI_V1* const User;

    /// Font management functions
    const FontAPI_V1* const Font;

    /// Mathematical functions
    const MathAPI_V1* const Math;

    /// Game lobby management functions
    const LobbyAPI_V1* const Lobby;
};

#pragma pack(pop)

/// @brief LetoAPI_V1 instance pointer
extern LIBRARIES_EXPORT const LetoAPI_V1* leto_api_v1;

static_assert(std::is_standard_layout<LetoAPI_V1>::value, "LetoAPI_V1 must be a standard layout type");

#endif
