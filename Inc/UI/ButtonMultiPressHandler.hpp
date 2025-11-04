/*
 * ButtonMultiPressHandler.hpp
 *
 *  Created on: Jan 11, 2026
 *      Author: Timur
 */

#ifndef INC_UI_BUTTON_MULTI_PRESS_HANDLER_HPP_
#define INC_UI_BUTTON_MULTI_PRESS_HANDLER_HPP_

#include <LetoAPI_V1/LetoAPI_V1.hpp> 

// Обработчик одновременного нажатия нескольких клавиш
class ButtonMultiPressHandler
{
protected:
	// Время, когда кнопка была нажата
	int count = 0;

	// Динамический массив с временем, когда была нажата та или иная кнопка
	int pressed_list[6]{};

	// Время зажатия кнопки для зажатия
	int hold_ms = 0;

public:
	ButtonMultiPressHandler(int count, int hold_ms)
		: count{ count }, hold_ms{ hold_ms }
	{
		if (this->count > (sizeof(pressed_list) / sizeof(pressed_list[0])))
			this->count = sizeof(pressed_list) / sizeof(pressed_list[0]);
	}

	// Зажато одновременно несколько кнопок
	bool MultiPressed(bool reset = true)
	{
		if (!pressed_list)
			return false;

		int min = pressed_list[0], max = pressed_list[0];

		for (int i = 1; i < count; ++i)
		{
			if (pressed_list[i] < min)
				min = pressed_list[i];
			else if (pressed_list[i] > max)
				max = pressed_list[i];
		}

		if (min == 0 || max == 0)
			return false;

		bool multi_pressed = max - min <= hold_ms;

		if (multi_pressed && reset)
		{
			for (int i = 0; i < count; ++i)
				Release(i);
		}

		return multi_pressed;
	}

	void Press(int button)
	{
		if (button >= count)
			return;
		pressed_list[button] = leto_api_v1->Globals->GetCurrentMs();
	}

	void Release(int button)
	{
		if (button >= count)
			return;
		pressed_list[button] = 0;
	}
};

#endif
