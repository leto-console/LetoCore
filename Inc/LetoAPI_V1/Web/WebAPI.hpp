/**
 * @file WebAPI_V1.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_API_HPP_
#define INC_LETO_API_V1_WEB_WEB_API_HPP_

#include <cstdint>
#include <type_traits>

#include <LetoAPI_V1/Web/WebConnection.hpp>
#include <LetoAPI_V1/Web/WebDeviceInfo.hpp>

#pragma pack(push, 4)

/// Network interaction functions
struct WebAPI_V1
{
	/**
	 * @brief Create connection
	 * 
	 * @param[out] connection Opened connection instance
	 * @param[in] channel Connection channel (COM, nRF, other)
	 * @param[in] port Connection port
	 * @param[in] id Device identifier for connection (0 for broadcast)
	 * @param[in] callback Callback function for data reception (can be NULL)
	 * 
	 * @return `true` if connection was successfully created
	 */
	bool (*const CreateConnection) (WebConnection_V1* connection, uint8_t channel, uint8_t port, uint32_t id, WebConnection_V1_Callback callback);
	
	/**
	 * @brief Close connection
	 *
	 * @param[in] connection Opened connection instance
	 */
	void (*const CloseConnection) (WebConnection_V1 connection);
	
	/**
	 * @brief Send data
	 *
	 * @param[in] connection Opened connection instance
	 * @param[in] data Data to send
	 * @param[in] size Size of the data to send
	 */
	void (*const SendData) (WebConnection_V1 connection, const void* data, uint32_t size);
	
	/**
	 * @brief Get list of nearby devices
	 * 
	 * @param[out] info Array to receive information
	 * @param[in] available Size of the destination array
	 *
	 * @return Number of devices
	 */
	uint32_t (*const GetDevicesNear) (WebDeviceInfo_V1* info, uint32_t available);

	/**
	 * @brief Find device by identifier
	 * 
	 * @param[in] id Device identifier
	 * @param[out] info Device information
	 * 
	 * @return `true` if device was successfully found
	 */
	bool (*const FindDeviceNear)(uint32_t id, WebDeviceInfo_V1* info);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<WebAPI_V1>::value, "WebAPI_V1 must be a standard layout type");

#endif
