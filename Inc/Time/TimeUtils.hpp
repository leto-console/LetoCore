/**
 * @file TimeUtils.hpp
 * @date Feb 12, 2025
 * @author Rakhimov T.
 */

#ifndef INC_TIME_TIMEUTILS_HPP_
#define INC_TIME_TIMEUTILS_HPP_

#include "LibrariesExport.h"

#include <stdint.h>

struct LETO_CORE_EXPORT TimeUtils
{
	static uint32_t count_in_sec;

#ifndef USE_HAL_DRIVER
    // Unix-время загрузки микроконтроллера
    static uint32_t start_ms;

    static void SetStartMs(uint32_t ms);
#endif
    /*
    	Инициализация точности отсчета системного времени
	*/
    static void Init(uint32_t count_in_sec);
    /* 
        Возвращает количество миллисекунд, прошедших с загрузки микроконтроллера 
    */
    static uint32_t GetCurrentMs();
    /*
        Возвращает количество микросекунд, прошедших с загрузки микроконтроллера
    */
    static uint32_t GetCurrentMks();

    /*
        Уснуть на заданное количество миллисекунд
    */
    static void SleepMs(uint32_t ms);

    /*
        Уснуть на заданное количество микросекунд
    */
    static void SleepMks(uint32_t mks);

};

#endif
