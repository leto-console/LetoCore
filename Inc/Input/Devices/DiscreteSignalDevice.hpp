/*
 * DiscreteSignalDevice.hpp
 *
 *  Created on: Jan 19, 2025
 *      Author: Timur
 */

#ifndef INC_INPUT_DEVICES_DISCRETESIGNAL_DEVICE_HPP_
#define INC_INPUT_DEVICES_DISCRETESIGNAL_DEVICE_HPP_

#include "LibrariesExport.h"

#ifdef USE_HAL_DRIVER

#include <HAL_include/HAL.hpp>

#endif

#include <Data/StaticText.hpp>
#include <Data/StaticTextView.hpp>
#include <Input/Devices/UserInputDevice.hpp>

class LIBRARIES_EXPORT DiscreteSignalDevice : public UserInputDevice
{
protected:
	StaticText32 name;

#ifdef USE_HAL_DRIVER
	GPIO_TypeDef* gpio_port;
	uint16_t gpio_pin;
#endif

	bool initialized = false;	// Флаг инициализации
	bool state = false;			// Состояние дискретного сигнала (0 или 1)

public:
	DiscreteSignalDevice();

#ifdef USE_HAL_DRIVER
	void	Init(const StaticText32& name, GPIO_TypeDef* gpio_port, uint16_t gpio_pin);
#else
	void	Init(const StaticText32& name);
#endif
	void	Tick();

	StaticTextView 	GetName() const;
	bool			GetState() const;
};

#endif /* INC_INPUT_DISCRETESIGNAL_HPP_ */
