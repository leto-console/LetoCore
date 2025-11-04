/**
 * @file LobbyInfoPacket.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_INFO_PACKET_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_INFO_PACKET_HPP_

#include <cstdint>

#include <LetoAPI_V1/Lobby/LobbyInfo.hpp>

#pragma pack(push, 1)

enum { FLP_INFO, FLP_MEMBERS_INFO };

/// @brief Пакет обмена информацией о подключениях
struct LobbyInfoPacket
{
    uint8_t type;

    union
    {
        // Используется при FLP_INFO
        LobbyInfo_V1 info;

        // Используется при FLP_USER_INFO
        struct 
        {
            uint8_t number;         ///< Номер 3-ки (0 или 1)
            uint32_t members[3];
        }
        user_info;
    };
};

static_assert(sizeof(LobbyInfoPacket) < 16);

#pragma pack(pop)

#endif
