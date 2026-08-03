/**
 * @file WebPacket.hpp
 * @date Jun 08, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_PACKET_HPP_
#define INC_LETO_API_V1_WEB_WEB_PACKET_HPP_

#include <cstdint>
#include <type_traits>
#include <cstring>

#include <Utils/crc16.hpp>

enum WP_FLAGS : uint8_t
{
	WP_FLAG_SYN = (0x01 << 0),	///< Synchronization flag (message awaits acknowledgment)
	WP_FLAG_ACK = (0x01 << 1),	///< Acknowledgment flag (response to synchronization flag)
};

#pragma pack(push, 1)

/**
 * @brief Low-level data packet
 */
struct WebPacket_V1
{
	/// [0...1] - Magic number for message validation
	uint8_t __magic[2] = { 0xC1, 0xA0 };
	
	/// [2...5] - Sender identifier
	uint32_t id_from;
	
	/// [6...9] - Receiver identifier
	uint32_t id_to;
	
	/// [10] - Port (system, game, synchronization, etc.)
	uint8_t port;

	/// [11...26] - Data (16 bytes)
	uint8_t data[16];

	/* Below are fields not included in CRC16 calculation */
	
	/// [27] - Flags `WP_FLAGS`
	uint8_t flags;
	
	/// [28] - Reserved
	uint8_t __reserved__;

	/// [29] - Remaining send attempts count
	uint8_t retries;

	/// [30...31] - CRC16
	uint8_t __crc16[2];

	bool operator==(const WebPacket_V1& packet) const
	{
		return memcmp(this, &packet, sizeof(this) - 5) == 0;
	}

	void operator=(const WebPacket_V1& packet)
	{
		memcpy(this, &packet, sizeof(WebPacket_V1));
	}

	inline void CalcCRC16()
	{
		// CRC16 big-endian
		uint16_t crc = calc_crc16(this, sizeof(WebPacket_V1) - 5);
		__crc16[0] = crc & 0xFF;
		__crc16[1] = (crc >> 8) & 0xFF;
	}

	inline bool CheckCRC16() const
	{
		uint16_t crc = __crc16[1] << 8 | __crc16[0];
		return crc == calc_crc16(this, sizeof(WebPacket_V1) - 5);
	}

	constexpr inline bool CompareCRC16(const WebPacket_V1& packet) const
	{
		return __crc16[0] == packet.__crc16[0] && __crc16[1] == packet.__crc16[1];
	}
};

#pragma pack(pop)

static_assert(std::is_standard_layout<WebPacket_V1>::value, "WebPacket_V1 must be standard_layout");
static_assert(sizeof(WebPacket_V1) == 32);

#endif /* INC_LETO_API_V1_WEB_WEB_PACKET_HPP_ */