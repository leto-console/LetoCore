/*
 * handle_commandline_from_UART.hpp
 *
 *  Created on: Dec 28, 2024
 *      Author: Timur
 */

#ifndef INC_UTILS_HANDLE_COMMANDLINE_FROM_UART_HPP_
#define INC_UTILS_HANDLE_COMMANDLINE_FROM_UART_HPP_

#ifdef USE_HAL_DRIVER

#include <HAL_include/HAL.hpp>

#define BUFFER_SIZE 32

class UARTCommandLine {
	UART_HandleTypeDef *huart				= nullptr;
	void(*	HandleCommandLine)(char*)		= nullptr;
	uint8_t uartReceivedSymbol 				= 0;
	char 	commandlineBuffer[BUFFER_SIZE] 	= {};
	uint8_t bufferIndex 					= 0;
	bool	skipNextSymbol 					= 0;
public:
	UARTCommandLine(UART_HandleTypeDef *huart, void(*HandleCommandLine)(char*) = nullptr);

	void Init();
	void SetCommmandLineHandler(void(*HandleCommandLine)(char*));
	void Callback();
};
#endif

#endif /* INC_UTILS_HANDLE_COMMANDLINE_FROM_UART_HPP_ */
