#include "SystemRestart.hpp"

#ifdef USE_HAL_DRIVER
#include <HAL_include/HAL.hpp>
#else if defined __WIN__
#include <Windows.h>
#endif


void SystemRestart()
{
#ifdef USE_HAL_DRIVER
    // 1. Отключаем все прерывания для безопасности
    __disable_irq();
    
    // 2. Вызываем системный сброс
    NVIC_SystemReset();
    
    // 3. Бесконечный цикл (код дальше не выполнится, контроллер уйдет в ресет)
    while(1)
    {
        __NOP();
    }
#else if defined(__WIN__)
    ExitProcess(0);
#endif
}