/**
 * @file LetoResult.hpp
 * @date May 27, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_APPLICATION_LETO_RESULT_HPP_
#define INC_LETO_API_V1_APPLICATION_LETO_RESULT_HPP_

#include <cstdint>

/// @brief Результат взаимодействия с приложением
using LetoResult_V1 = int32_t;

constexpr LetoResult_V1 LETO_V1_SUCCESS 		=  0;	///< Выполнено успешно
constexpr LetoResult_V1 LETO_V1_ERROR 			= -1;	///< Ошибка
constexpr LetoResult_V1 LETO_V1_UNSUPPORT 		= -2;	///< Не поддерживается
constexpr LetoResult_V1 LETO_V1_ARGS_ERROR		= -3;	///< Ошибка в аргументах
constexpr LetoResult_V1 LETO_V1_ALREADY_DONE 	= -4;	///< Выполнялось ранее
constexpr LetoResult_V1 LETO_V1_OUT_OF_MEMORY	= -5;	///< Не хватает динамической памяти

#endif
