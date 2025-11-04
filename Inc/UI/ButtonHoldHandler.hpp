/*
 * ButtonHoldHandler.hpp
 *
 *  Created on: Jan 11, 2026
 *      Author: Timur
 */

#ifndef INC_UI_BUTTON_HOLD_HANDLER_HPP_
#define INC_UI_BUTTON_HOLD_HANDLER_HPP_

#include <LetoAPI_V1/LetoAPI_V1.hpp> 

// Обработчик удерживания кнопки
class ButtonHoldHandler
{
public:
	enum class OnHoldPolicy 
	{
		KEEP,		// Сохраняет значение Hold до его сброса кнопкой
		RELEASE,	// После достижения состояния Hold дальнейшая обработка прерывается
		PRESS,		// После достижения состояния Hold обработка начинается заново
	};

protected:
	// Время, когда кнопка была нажата
	uint32_t pressed = 0;

	uint32_t counter = 0;

public:
	ButtonHoldHandler() = default;

	// Кнопка удерживается (при reset = false, до момента Release будет возвращаться true, иначе - один раз)
	bool Holded(uint32_t hold_ms, OnHoldPolicy policy = OnHoldPolicy::RELEASE)
	{
		bool holded = pressed > 0 && leto_api_v1->Globals->GetCurrentMs() - pressed > hold_ms;
		if (holded)
		{
			if (policy == OnHoldPolicy::RELEASE)
				Release();
			else if (policy == OnHoldPolicy::PRESS)
				Press();
		}
		return holded;
	}

	// Кнопка удерживается в многоступенчатом режиме
	bool WaitHolded(uint32_t first_hold_ms, uint32_t hold_ms)
	{
		if (counter == 0)
			return false;
		else if (counter == 1)
			return Holded(first_hold_ms, OnHoldPolicy::PRESS);
		else
			return Holded(hold_ms, OnHoldPolicy::PRESS);
	}

	void Press()
	{
		counter++;
		pressed = leto_api_v1->Globals->GetCurrentMs();
	}

	void Release()
	{
		counter = 0;
		pressed = 0;
	}
};

#endif
