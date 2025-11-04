/*
 * ButtonDevice.cpp
 *
 *  Created on: Oct 20, 2025
 *      Author: Timur
 */

#include <Input/Devices/ButtonDevice.hpp>
#include <Input/ButtonEvent.hpp>

#include <Time/TimeUtils.hpp>

#ifndef USE_HAL_DRIVER
#include <Utils/FileConfig.hpp>
#endif

#include <string.h>

const long long debounce_ms = 15;

ButtonDevice::ButtonDevice()
	: UserInputDevice{ AE_Button }
{
}

#ifdef USE_HAL_DRIVER
void ButtonDevice::Init(uint8_t id, GPIO_TypeDef* gpio_port, uint16_t gpio_pin)
{
	this->gpio_port = gpio_port;
	this->gpio_pin = gpio_pin;
	UserInputDevice::Init(id);
}
#else
void ButtonDevice::Init(uint8_t id, uint64_t key)
{
	this->keys.push_back(key);
	this->key = key;
	UserInputDevice::Init(id);
	// Нужно для управления приложением с помощью конфигурационного файла
	// Пока что для отладки хватает и нажатия на клавиатуру
	//FileConfig::InitBool("buttons", name);
}
#endif

void ButtonDevice::Press()
{
	stable = true;
	click_time = TimeUtils::GetCurrentMs();
	PushEvent((uint32_t) ButtonEvent::PRESSED);
}

void ButtonDevice::Release()
{
	stable = false;
	PushEvent((uint32_t)ButtonEvent::RELEASED);
}

void ButtonDevice::Tick()
{
	if (!initialized)
		return;

	long long current_time = TimeUtils::GetCurrentMs();

#ifdef USE_HAL_DRIVER

	current_state = !HAL_GPIO_ReadPin(gpio_port, gpio_pin);

	if (!stable && current_state)
	{
		if (!check)
		{
			check = true;
		}
		else if (current_time - click_time > debounce_ms)
		{
			check = false;
			Press();
		}
	}
	else
	{
		check = false;
	}

#elif defined (__WIN__)

	int winAPIKeyEvent = 0;

	if (WinAPIEventsMap.find(key) != WinAPIEventsMap.end() &&
		!WinAPIEventsMap[key].empty())
	{
		WinAPIKeyEvent& event = WinAPIEventsMap[key].front();
		winAPIKeyEvent = event.event;
		WinAPIEventsMap[key].pop_front();
	}

	if		(winAPIKeyEvent == WinAPIKeyEvent::PRESSED)		current_state = true;
	else if (winAPIKeyEvent == WinAPIKeyEvent::RELEASED)	current_state = false;

	if (current_state && !stable)
	{
		stable = true;
		Press();
	}

#endif

	if (!stable) return;

	if (!current_state)
		Release();
}
