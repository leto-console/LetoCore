/*
 * ButtonEvent.hpp
 *
 *  Created on: Nov 04, 2025
 *      Author: Timur
 */

#ifndef INC_INPUT_BUTTON_EVENT_HPP_
#define INC_INPUT_BUTTON_EVENT_HPP_

#include <stdint.h>
#include <ABI/AppEvent.hpp>

namespace ButtonEvent
{
	enum : uint32_t
	{
		IDLE = 0,
		PRESSED,		// Кнопка нажата
		RELEASED		// Кнопка отпущена
	};

	inline bool Compare(const AppEvent& e1, const AppEvent& e2)
	{
		if (e1.source != AE_Button) return false;
		return e1.source == e2.source && e1.id == e2.id && e1.data == e2.data;
	}

	inline bool IsReleased(const AppEvent& event)
	{
		return event.source == AE_Button && event.data == RELEASED;
	}

	inline bool IsPressed(const AppEvent& event, bool pressed = true)
	{
		if (!pressed)
			return IsReleased(event);
		return event.source == AE_Button && event.data == PRESSED;
	}
}


#endif
