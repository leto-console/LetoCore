/**
 * @file LobbyConnection.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_CONNECTION_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_CONNECTION_HPP_

#include <cstdint>

const uint8_t LC_MEMBERS_COUNT = 6;

enum LC_State : uint8_t
{
	LC_STATE_DISCONNECTED 	= 0x00,	///< No connection
	LC_STATE_JOINING		= 0x01,	///< Connection in progress (entering lobby)
	LC_STATE_CONNECTED		= 0x02,	///< Connection established
};

#pragma pack(push, 2)

/**
 * @brief Callback for data reception handling
 *
 * @param[in] id Identifier of the sender
 * @param[in] data Received data buffer
 * @param[in] size Size of the received data
 */
using LobbyConnection_V1_Callback = void (*)(uint32_t id, const void* data, uint32_t size);

/**
 * @brief Lobby connection data structure
 */
struct LobbyConnection_V1
{
	/// Lobby creator (owner)
	uint32_t owner;
    
	/// Application identifier
	uint16_t app_id;

	/// Number of connected members (including creator)
	uint16_t connected;

	/// Maximum allowed number of members
	uint16_t max_count;

	/// Lobby members
	uint32_t members[LC_MEMBERS_COUNT];
	
	/// Callback for data reception handling
	LobbyConnection_V1_Callback callback;

	/// Connection state
	uint8_t state;
};

#pragma pack(pop)

#endif /* INC_LETO_API_V1_LOBBY_LOBBY_CONNECTION_HPP_ */
