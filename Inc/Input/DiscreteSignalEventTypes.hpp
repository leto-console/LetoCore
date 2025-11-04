/*
 * DiscreteSignalEventTypes.hpp
 *
 *  Created on: Nov 04, 2025
 *      Author: Timur
 */

#ifndef INC_INPUT_DISCRETE_SIGNAL_EVENT_TYPES_HPP_
#define INC_INPUT_DISCRETE_SIGNAL_EVENT_TYPES_HPP_

#include <stdint.h>

enum class DiscreteSignalEventType : uint8_t
{
	_IDLE = 0,
	_TRUE,			// Дискретная команда переключена в состояние 1 (TRUE)
	_FALSE			// Дискретная команда переключена в состояние 0 (FALSE)
};

#endif
