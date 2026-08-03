#ifdef USE_HAL_DRIVER

#include <UART/UART_console_init.hpp>

UART_HandleTypeDef *huart_log_console = NULL;
bool uart_log_enabled = false;

// --- Включить UART ---
void uart_log_enable(void)
{
	uart_log_enabled = true;
	__HAL_RCC_USART2_CLK_ENABLE();   // для USART2
}

void uart_log_disable(void)
{
	uart_log_enabled = false;
	__HAL_RCC_USART2_CLK_DISABLE();   // для USART2
}

void uart_log_init(UART_HandleTypeDef *huart)
{
	uart_log_enabled = true;
	huart_log_console = huart;
}

#endif
