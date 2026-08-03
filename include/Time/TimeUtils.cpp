/*
 * TimeUtils.cpp
 *
 *  Created on: Feb 12, 2025
 *      Author: Timur
 */

#include "Time/TimeUtils.hpp"

uint32_t TimeUtils::count_in_sec = 1000;

#ifndef USE_HAL_DRIVER
#include <chrono>

using namespace std::chrono;

uint32_t TimeUtils::start_ms = 0;

void TimeUtils::SetStartMs(uint32_t ms)
{
    start_ms = ms;
}

#else

#include <HAL_include/HAL.hpp>

#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC

#endif

#if defined(__WIN__)

#include <Windows.h>

#endif

void TimeUtils::Init(uint32_t count_in_sec)
{
    static bool inited = false;
    if (inited || (count_in_sec == 0))
    {
        return;
    }
	TimeUtils::count_in_sec = count_in_sec;

#ifdef USE_HAL_DRIVER
	HAL_SYSTICK_Config( HAL_RCC_GetHCLKFreq() / count_in_sec );

    SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
	DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;   // запускаем счётчик
#endif

    inited = true;
}

uint32_t TimeUtils::GetCurrentMs()
{
#ifdef USE_HAL_DRIVER
    return HAL_GetTick() / (count_in_sec / 1000.0f);
#else	
    return ((uint32_t) duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - start_ms); /// (count_in_sec / 1000.0);
#endif
}
uint32_t TimeUtils::GetCurrentMks()
{
#ifdef USE_HAL_DRIVER
    return HAL_GetTick() / (count_in_sec / 1000000.0f);
#else	
    return ((uint32_t) duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - (uint64_t) start_ms * 1000); /// (count_in_sec / 1000.0);
#endif
}

void TimeUtils::SleepMs(uint32_t ms)
{
#if defined(__WIN__)
    uint32_t cur_ms = GetCurrentMs();
    while (GetCurrentMs() - cur_ms < ms);
#endif
}

void TimeUtils::SleepMks(uint32_t mks)
{
#if defined(WIN32)
    uint32_t cur_mks = GetCurrentMks();
    while (GetCurrentMks() - cur_mks < mks);
#else
    uint32_t us_count_tic =  mks * (HAL_RCC_GetHCLKFreq() / 1000000);
    DWT->CYCCNT = 0U; // обнуляем счётчик
    while (DWT->CYCCNT < us_count_tic);
#endif
}
