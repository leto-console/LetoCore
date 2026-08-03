/*
 * handle_commandline_from_UART.cpp
 *
 *  Created on: Dec 28, 2024
 *      Author: Timur
 */

#include "UART/UART_CommandLineCallback.hpp"

#ifdef USE_HAL_DRIVER

#include <stdio.h>
#include <string.h>

UARTCommandLine::UARTCommandLine(UART_HandleTypeDef *huart, void(*HandleCommandLine)(char*)) :
		huart{ huart }, HandleCommandLine{ HandleCommandLine }
{
}

void UARTCommandLine::Init()
{
	HAL_UART_Receive_IT(huart, &uartReceivedSymbol, 1);
}

void UARTCommandLine::SetCommmandLineHandler(void(*HandleCommandLine)(char*))
{
	this->HandleCommandLine = HandleCommandLine;
}

void UARTCommandLine::Callback()
{
	HAL_UART_Receive_IT(huart, &uartReceivedSymbol, 1);
	if (skipNextSymbol)
	{
		skipNextSymbol = 0;
		return;
	}
	else if (uartReceivedSymbol == '\r')
	{
		HAL_UART_Transmit(huart, &uartReceivedSymbol, 1, HAL_MAX_DELAY);
		uartReceivedSymbol = '\n';
		HAL_UART_Transmit(huart, &uartReceivedSymbol, 1, HAL_MAX_DELAY);

		commandlineBuffer[bufferIndex] = 0;
		bufferIndex = 0;

		if(HandleCommandLine != nullptr)
			HandleCommandLine(commandlineBuffer);
		printf("\r\n");

		if (strcmp(commandlineBuffer, "write") == 0) // равно
			printf("Команда на запись\r\n");
	}
	else if (uartReceivedSymbol == 127)
	{
		if (bufferIndex > 0)
			bufferIndex--;
		HAL_UART_Transmit(huart, &uartReceivedSymbol, 1, HAL_MAX_DELAY);
	}
	else if (uartReceivedSymbol >= 32 && uartReceivedSymbol <= 126)
	{
		if (uartReceivedSymbol == 91)
		{
			skipNextSymbol = 1;
			return;
		}
		if (bufferIndex < BUFFER_SIZE)
			commandlineBuffer[bufferIndex++] = uartReceivedSymbol;
		HAL_UART_Transmit(huart, &uartReceivedSymbol, 1, HAL_MAX_DELAY);
	}
}

#endif
