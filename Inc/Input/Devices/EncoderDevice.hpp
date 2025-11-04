/*
 * Encoder.hpp
 *
 *  Created on: Nov 08, 2025
 *      Author: Timur
 */

#ifndef INC_INPUT_ENCODER_HPP_
#define INC_INPUT_ENCODER_HPP_

#include "LibrariesExport.h"

#ifdef USE_HAL_DRIVER

#include <HAL_include/HAL.hpp>

#endif

#include <Input/Devices/UserInputDevice.hpp>

class LIBRARIES_EXPORT EncoderDevice : public UserInputDevice
{
protected:
	bool reverse = false;

#ifdef USE_HAL_DRIVER
	GPIO_TypeDef* gpio_clk_port;
	uint16_t gpio_clk_pin;

	GPIO_TypeDef* gpio_dt_port;
	uint16_t gpio_dt_pin;

	bool stateCLK{};
	bool stateDT{};
	bool lastStateCLK{};
#else
	uint64_t key_left{}, key_right{};
#endif

	bool state = false;			// Состояние дискретного сигнала (0 или 1)

public:
	EncoderDevice();

#ifdef USE_HAL_DRIVER
	void	Init(uint8_t id, GPIO_TypeDef* gpio_clk_port, uint16_t gpio_clk_pin, GPIO_TypeDef* gpio_dt_port, uint16_t gpio_dt_pin);
#else
	void	Init(uint8_t id, uint64_t key_left, uint64_t key_right);
#endif
	void	Tick();

	void SetReverse(bool reverse = true);

	virtual void OnLeftTurn(uint8_t count = 1);
	virtual void OnRightTurn(uint8_t count = 1);
};

#endif /* INC_INPUT_DISCRETESIGNAL_HPP_ */
