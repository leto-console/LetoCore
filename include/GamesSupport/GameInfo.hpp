/**
 * @file GameInfo.hpp
 * @date May 25, 2026
 * @author Rakhimov T.
 */

#ifndef INC_GAME_SUPPORT_GAME_INFO_HPP_
#define INC_GAME_SUPPORT_GAME_INFO_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <GamesSupport/BaseGame.hpp>

/**
 * @brief Информация об игре, используемая для временного хранения
 */
struct GameInfo
{
	/// Версия API, на котором написана игра (major)
	uint16_t api_version;
	
	/// Уникальный идентификатор игры
	uint16_t id;
	
	/// Название игры на английском
	char en_name[32] {};
	
	/// Название игры на русском
	char ru_name[32] {};
	
	/// Путь до файла с игрой
	char path[64] {};
};

static_assert(std::is_standard_layout<GameInfo>::value, "GameInfo должен быть standard_layout");

#endif
