/**
 * @file SystemMode.hpp
 * @date Feb 21, 2026
 * @author Timur
 */

#ifndef INC_SYSTEM_SYSTEM_MODE_HPP_
#define INC_SYSTEM_SYSTEM_MODE_HPP_

#include "LibrariesExport.h"

/// Режим работы приложения
enum class SystemMode : int
{
	NONE	= 0,			///< Отсутствие режима
	AUTH	= (1 << 0),		///< Режим авторизации
	ADMIN	= (1 << 1),		///< Режим администратора
	USER 	= (1 << 2)		///< Пользовательский режим
};

/**
 * @brief Установить режим работы системы
 */
extern LIBRARIES_EXPORT void SetSystemMode(SystemMode mode);

/**
 * @brief Получить режим работы системы
 */
extern LIBRARIES_EXPORT SystemMode GetSystemMode();

/**
 * @brief Признак изменения режима работы системы
 */
extern LIBRARIES_EXPORT bool SystemModeChanged();

#endif
