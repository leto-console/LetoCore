/**
 * @file WebConnection.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_CONNECTION_HPP_
#define INC_LETO_API_V1_WEB_WEB_CONNECTION_HPP_

#include <cstdint>

const uint8_t WC_CHANNEL_ALL 	= 0x00;				///< Connection with all devices
const uint8_t WC_CHANNEL_UART 	= 0x01;				///< Connection via UART
const uint8_t WC_CHANNEL_NRF 	= 0x02;				///< Connection via nRF
const uint8_t WC_CHANNEL_MAX 	= WC_CHANNEL_NRF;	///< Maximum channel variety

const uint8_t WC_PORT_PING		= 0x01;		///< Port for ping requests
const uint8_t WC_PORT_INFO		= 0x02;		///< Information port (heartbeat)
const uint8_t WC_PORT_SYSTEM 	= 0x20;		///< Initial port for system ports (0x20...0x60 - 64 ports)
const uint8_t WC_PORT_INF_LOBBY	= 0x70;		///< Port for lobby discovery and information exchange
const uint8_t WC_PORT_CON_LOBBY	= 0x77;		///< Port for interaction within game lobbies
const uint8_t WC_PORT_APP 		= 0x80;		///< Initial port for application interaction (0x80...0xC0 - 64 ports)


#pragma pack(push, 4)

/**
 * @brief Callback for data reception handling
 *
 * @param[in] channel Channel of the sender
 * @param[in] port Port of the sender
 * @param[in] id Identifier of the sender
 * @param[in] data Received data buffer
 * @param[in] size Size of the received data
 */
using WebConnection_V1_Callback = void (*)(uint8_t channel, uint8_t port, uint32_t id, const void* data, uint32_t size);

/**
 * @brief Connection data structure
 */
struct WebConnection_V1
{
	/// Connection channel (COM, nRF, other) (0 for all)
	uint8_t channel;
	
	/// Connection port (system message, game message, etc.)
	uint8_t port;
	
	/// Identifier of the target device (0 for broadcast)
	uint32_t id;
	
	/// Callback for data reception handling
	WebConnection_V1_Callback callback;

	/// Reserved for future function expansion
	uint32_t _reserved_[5];
};

#pragma pack(pop)

#endif /* INC_LETO_API_V1_WEB_WEB_CONNECTION_HPP_ */
