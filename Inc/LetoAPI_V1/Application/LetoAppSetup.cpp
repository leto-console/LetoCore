#include "LetoAppSetup.hpp"

#include "../LetoAPI_V1.hpp"

#ifdef USE_HAL_DRIVER

extern "C" 
{
    extern uint32_t _sbss; // Начало .bss
    extern uint32_t _ebss; // Конец .bss

    typedef void (*pFunc)(void);

    extern pFunc __init_array_start[]; // Начало .init_array
    extern pFunc __init_array_end[];   // Конец .init_array
}

void __attribute__((optimize("O0"))) clear_bss(void)
{
    uint32_t *dst = &_sbss;
    while (dst < &_ebss) 
    {
        *dst++ = 0;
    }
}

void __attribute__((optimize("O0"))) init_array(void) 
{
    size_t count = __init_array_end - __init_array_start;
    for (size_t i = 0; i < count; i++) 
    {
        __init_array_start[i](); // Вызываем каждый скрытый конструктор
    }
}

#endif


LetoResult_V1 LetoAppSetup(const LetoAPI_V1 *api)
{
#ifdef USE_HAL_DRIVER
    clear_bss();
    init_array();
#endif

    if (!api || api->Version->major != 1) return LETO_V1_ARGS_ERROR;
    leto_api_v1 = api;

    return LETO_V1_SUCCESS;
}
