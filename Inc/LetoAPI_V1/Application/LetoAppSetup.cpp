#include "LetoAppSetup.hpp"

#include "../LetoAPI_V1.hpp"

#ifdef USE_HAL_DRIVER

extern "C" 
{
    extern uint32_t _sbss; // Start of .bss section
    extern uint32_t _ebss; // End of .bss section

    typedef void (*pFunc)(void);

    extern pFunc __init_array_start[]; // Start of .init_array section
    extern pFunc __init_array_end[];   // End of .init_array section
}

/**
 * @brief Zero out .bss section memory
 */
void __attribute__((optimize("O0"))) clear_bss(void)
{
    uint32_t *dst = &_sbss;
    while (dst < &_ebss) 
    {
        *dst++ = 0;
    }
}

/**
 * @brief Call global and static constructors
 */
void __attribute__((optimize("O0"))) init_array(void) 
{
    size_t count = __init_array_end - __init_array_start;
    for (size_t i = 0; i < count; i++) 
    {
        __init_array_start[i](); // Call each static constructor
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
