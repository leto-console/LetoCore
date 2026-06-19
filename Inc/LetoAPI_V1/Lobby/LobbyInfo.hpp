/**
 * @file LobbyInfo.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_INFO_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_INFO_HPP_

#include <cstdint>

#pragma pack(push, 2)

/**
 * @brief Game lobby information
 */
struct LobbyInfo_V1
{
    /// Lobby creator (owner)
    uint32_t owner;
    
	/// Application identifier
	uint16_t app_id;

    /// Number of connected members (including creator)
    uint16_t connected;

    /// Maximum allowed number of members
    uint16_t max_count;

    /// Timestamp of the last room information transmission
    uint32_t last_info;
};

#pragma pack(pop)

#endif /* INC_LETO_API_V1_LOBBY_LOBBY_INFO_HPP_ */
