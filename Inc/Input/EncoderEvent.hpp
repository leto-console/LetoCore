/*
 * DiscreteSignalEventTypes.hpp
 *
 *  Created on: Nov 04, 2025
 *      Author: Timur
 */

#ifndef INC_INPUT_ENCODER_EVENT_TYPES_HPP_
#define INC_INPUT_ENCODER_EVENT_TYPES_HPP_

#include <stdint.h>
#include <ABI/AppEvent.hpp>

namespace EncoderEvent
{
	enum : uint8_t
	{
		IDLE = 0,
		TURN_RIGHT,		// По часовой стрелке (направо относительно верхней точки)
		TURN_LEFT,		// Против часовой стрелки (налево относительно верхней точки)
	};

	inline bool Compare(const AppEvent& e1, const AppEvent& e2)
	{
		if (e1.source != AE_Encoder) return false;
		return e1.source == e2.source && e1.id == e2.id && e1.data == e2.data;
	}

	inline bool IsTurned(const AppEvent& event)
	{
		return event.source == AE_Encoder && event.data != IDLE;
	}

	inline bool IsTurnedLeft(const AppEvent& event)
	{
		if (!IsTurned(event)) return false;
		return event.data == TURN_LEFT;
	}

	inline bool IsTurnedRight(const AppEvent& event)
	{
		if (!IsTurned(event)) return false;
		return event.data == TURN_RIGHT;
	}

}

#endif
