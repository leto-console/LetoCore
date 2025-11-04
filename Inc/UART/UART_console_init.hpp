/*
 * UART_console_init.hpp
 *
 *  Created on: Dec 20, 2025
 *      Author: Timur
 */

#ifndef INC_UART_CONSOLE_INIT_HPP_
#define INC_UART_CONSOLE_INIT_HPP_

#ifdef USE_HAL_DRIVER

#ifdef __cplusplus
extern "C"{
#endif

#include <stm32f4xx_hal.h>
#include "stdio.h"
#include "stdbool.h"

extern UART_HandleTypeDef *huart_log_console;
extern bool uart_log_enabled;

void uart_log_init(UART_HandleTypeDef *huart);
void uart_log_enable(void);
void uart_log_disable(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
