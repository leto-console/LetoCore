/**
 * @file AppEvent.hpp
 * @date Jun 02, 2026
 * @author Rakhimov T.
 */

#ifndef INC_ABI_APP_EVENT_HPP_
#define INC_ABI_APP_EVENT_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <ABI/AppEventSources.hpp>

#pragma pack(push, 4)

/**
 * @brief Событие для приложения
 */
struct AppEvent
{
	/// Источник события
	uint16_t source;
	
	/// Идентификатор события
	uint16_t id;
	
	/// Данные события
	uint32_t data;
};

#pragma pack(pop)

/**
 * @brief Тип функции для сравнения двух событий
 */
using AppEventComparator = bool(*)(const AppEvent& e1, const AppEvent& e2);

#endif /* INC_ABI_APP_EVENT_HPP_ */
