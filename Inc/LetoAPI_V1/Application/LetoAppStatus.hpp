/**
 * @file LetoAppStatus.hpp
 * @date May 27, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_APPLICATION_LETO_APP_STATUS_HPP_
#define INC_LETO_API_V1_APPLICATION_LETO_APP_STATUS_HPP_

#include <cstdint>

/// @brief Статус приложения
using LetoAppStatus_V1 = int32_t;

constexpr LetoAppStatus_V1 LETO_V1_WORK_STATUS 		=  0;	///< Штатная работа приложения
constexpr LetoAppStatus_V1 LETO_V1_CLOSE_STATUS		= -1;	///< Запрос на выход из приложения
constexpr LetoAppStatus_V1 LETO_V1_ERROR_STATUS 	= -2;	///< Выброс ошибки в ходе работы приложения
constexpr LetoAppStatus_V1 LETO_V1_NOT_WORK_STATUS  = -3;	///< Приложение не запущено


#endif
