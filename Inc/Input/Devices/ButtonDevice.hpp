/*
 * Button.hpp
 *
 *  Created on: Oct 20, 2025
 *      Author: Timur
 */

#ifndef INC_INPUT_DEVICES_BUTTON_DEVICE_HPP_
#define INC_INPUT_DEVICES_BUTTON_DEVICE_HPP_

#include "LibrariesExport.h"

#include <string>

#ifdef USE_HAL_DRIVER

#include <HAL_include/HAL.hpp>

#endif

#include <Input/Devices/UserInputDevice.hpp>

class LIBRARIES_EXPORT ButtonDevice : public UserInputDevice
{
protected:
	bool current_state = false;
	bool stable = false;
	bool check = false;
	long long click_time = 0;

#ifdef USE_HAL_DRIVER
	GPIO_TypeDef* gpio_port{};
	uint16_t gpio_pin{};
#else
	uint64_t key{};
#endif

public:
	ButtonDevice();

#ifdef USE_HAL_DRIVER
	void	Init(uint8_t id, GPIO_TypeDef* gpio_port, uint16_t gpio_pin);
#else
	void	Init(uint8_t id, uint64_t key);
	uint64_t GetKey() const { return key; }
#endif

	virtual void Press();
	virtual void Release();

	void Tick();
};

#endif /* INC_INPUT_BUTTON_HPP_ */
