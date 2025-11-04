/**
 * @file AppBinHeader.hpp
 * @date May 27, 2026
 * @author Rakhimov T.
 */

#ifndef INC_ABI_APP_BIN_HEADER_HPP_
#define INC_ABI_APP_BIN_HEADER_HPP_

#include <cstdint>
#include <type_traits>

/// Бинарный файл обычного типа
#define LETO_EXECUTABLE_TYPE_BIN "LETO_EXEC_BIN"

/// Бинарный файл игрового типа
#define LETO_EXECUTABLE_TYPE_GAME "LETO_EXEC_GAME"

#define LETO_EXECUTABLE_INFO_VAR_NAME __leto_exec_info__
#define LETO_EXECUTABLE_INFO_VAR_TEXT "__leto_exec_info__"

#pragma pack(push, 4)

/**
 * @brief Информация об исполняемом бинарном файле (приложении)
 */
struct AppBinHeader
{
	/// "Магическое" число для проверки
	char type[16] { LETO_EXECUTABLE_TYPE_BIN };

	/// Версия API, на котором написано приложение (major)
	uint16_t api_version;

	/// Уникальный идентификатор приложения
	uint16_t id;
		
	/// Название приложения на английском
	char en_name[32] {};
	
	/// Название приложения на русском
	char ru_name[32] {};
};

#pragma pack(pop)

static_assert(std::is_standard_layout<AppBinHeader>::value, "AppBinHeader должен быть standard_layout");

#endif /* INC_ABI_APPLICATION_BINARY_HEADER_HPP_ */
