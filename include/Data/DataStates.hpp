/**
 * @file DataStates.hpp
 * @date Oct 19, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_SOURCE_DATA_STATES_HPP_
#define INC_DATA_SOURCE_DATA_STATES_HPP_

//#include <ctype.h>
#include <stdint.h>

enum class DataStates : uint8_t
{
	__NO_DATA,		// Нет данных
	__OK,			// Получено и актульно
	__TIMEOUT,		// Не получено: истек таймаут
	__FAILURE		// Отказ по данному параметру
};

#endif /* INC_DATA_SOURCE_DATA_STATES_HPP_ */
