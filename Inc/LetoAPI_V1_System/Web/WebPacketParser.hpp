/**
 * @file WebPacketParser.hpp
 * @date Jun 06, 2026
 * @author Timur
 */

#ifndef INC_LETO_API_V1_WEB_WEB_PACKET_PARSER_HPP_
#define INC_LETO_API_V1_WEB_WEB_PACKET_PARSER_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <Data/StaticListView.hpp>

struct WebPacketParser
{
	static LIBRARIES_EXPORT void CallbackByte(uint8_t channel, uint8_t byte);
};

#endif /* INC_LETO_API_V1_WEB_WEB_PACKET_PARSER_HPP_ */
