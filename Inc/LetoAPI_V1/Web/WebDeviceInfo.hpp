/**
 * @file WebDeviceInfo.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_DEVICE_INFO_HPP_
#define INC_LETO_API_V1_WEB_WEB_DEVICE_INFO_HPP_

#include <cstdint>

#pragma pack(push, 4)

/**
 * @brief Network device information
 */
struct WebDeviceInfo_V1
{
	/// Connection channel (COM, nRF, other)
	uint8_t channel;

	/// Identifier
	uint32_t id;
		
	/// Avatar identifier
	uint32_t avatar_id;

	/// Active application identifier
	uint32_t app_id;
	
	/// Network name (UTF-8)
	char web_name[32];

	/// Lobby connection ownership status
	uint32_t lobby_owner;

	/// Lobby connection request status
	uint32_t joining_lobby;
};

#pragma pack(pop)

#endif /* INC_LETO_API_V1_WEB_WEB_DEVICE_INFO_HPP_ */
