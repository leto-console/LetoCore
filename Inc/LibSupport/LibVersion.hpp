/*
 *	LibHeader.hpp
 *
 *	Created on: Mar 12, 2026
 *		Author: Timur
 */

#ifndef INC_LIBS_SUPPORT_LIB_HEADER_HPP_
#define INC_LIBS_SUPPORT_LIB_HEADER_HPP_

#include "LibrariesExport.h"

#include <type_traits>
#include <cstdint>

// Заголовой библиотеки (версия)
struct LIBRARIES_EXPORT LibVersion
{
	// Уникальное значение для идентификации библиотеки
	const uint8_t unique[4] = { 0xC1, 0xA0, 0xBA, 0xBE };

	// Изменяется при внедрении обратно несовместимых функций
	uint16_t major;
	
	// Изменяется при внедрении новых функций
	uint16_t minor;
	
	LibVersion() = default;

	LibVersion(uint16_t major, uint16_t minor)
		: major{ major }, minor{ minor }
	{ }
};

static_assert(std::is_standard_layout<LibVersion>::value, "LibVersion должен быть standard_layout");

#endif
