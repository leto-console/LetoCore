/*
 * printf_prototype_for_UART.hpp
 *
 *  Created on: Dec 28, 2024
 *      Author: Timur
 */

#ifndef INC_PRINTF_PROTOTYPE_FOR_UART_HPP_
#define INC_PRINTF_PROTOTYPE_FOR_UART_HPP_

#include <UART/UART_console_init.hpp>

#ifdef __cplusplus
extern "C"{
#endif

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#include <cstdio>
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  if (!uart_log_enabled)
	  return ch;
  static char r = '\r';
  if (ch == '\n') HAL_UART_Transmit(huart_log_console, (uint8_t *)&r, 1, 1000);
  HAL_UART_Transmit(huart_log_console, (uint8_t *)&ch, 1, 1000);
  return ch;
}

#ifdef __cplusplus
}
#endif

#endif /* INC_PRINTF_PROTOTYPE_FOR_UART_HPP_ */
