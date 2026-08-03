/**
 * @file LobbyInfoPacket.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_INFO_PACKET_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_INFO_PACKET_HPP_

#include <cstdint>

#include <LetoAPI_V1/Lobby/LobbyInfo.h>

#pragma pack(push, 1)

enum { FLP_INFO, FLP_MEMBERS_INFO };

/// @brief Connection information exchange packet
struct LobbyInfoPacket
{
    uint8_t type;

    union
    {
        // Used when type is FLP_INFO
        LobbyInfo_V1 info;

        // Used when type is FLP_MEMBERS_INFO
        struct 
        {
            uint8_t number;         ///< Index of user triad (0 or 1)
            uint32_t members[3];
        }
        user_info;
    };
};

static_assert(sizeof(LobbyInfoPacket) < 16);

#pragma pack(pop)

#endif
