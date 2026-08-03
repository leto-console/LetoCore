/*
 * DiscreteSignalDevice.cpp
 *
 *  Created on: Jan 19, 2025
 *      Author: Timur
 */

#include "Input/Devices/DiscreteSignalDevice.hpp"

#ifndef USE_HAL_DRIVER
#include "Utils/FileConfig.hpp"
#endif

#include <string.h>

DiscreteSignalDevice::DiscreteSignalDevice()
	: UserInputDevice{ AE_Discrete }, name{ "N/A" }, initialized{ false }
{
}

#ifdef USE_HAL_DRIVER
void DiscreteSignalDevice::Init(const StaticText32& name, GPIO_TypeDef* gpio_port, uint16_t gpio_pin)
{
	this->name			= name;
	this->gpio_port 	= gpio_port;
	this->gpio_pin		= gpio_pin;

	// TODO: На будущее, спроектировать правильнее систему
	UserInputDevice::Init(0);
}
#else
void DiscreteSignalDevice::Init(const StaticText32& name)
{
	this->name			= name;
	// TODO: На будущее, спроектировать правильнее систему
	UserInputDevice::Init(0);
	FileConfig::InitBool("discrete_signals", name.ConstChar());
}
#endif

void DiscreteSignalDevice::Tick()
{
	if (!initialized)
		return;

#ifdef USE_HAL_DRIVER
	state = !HAL_GPIO_ReadPin(gpio_port, gpio_pin);
#else
	FileConfig::ReadBool(&state, "discrete_signals", name.ConstChar());
#endif
}

StaticTextView DiscreteSignalDevice::GetName() const
{
	return this->name;
}

bool DiscreteSignalDevice::GetState() const
{
	return state;
}
