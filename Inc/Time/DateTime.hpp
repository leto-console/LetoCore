/*
 * DateTime.hpp
 *
 *  Created on: Mar 19, 2026
 *      Author: Timur
 */

#include <stdint.h>

#ifndef INC_TIME_DATE_TIME_HPP_
#define INC_TIME_DATE_TIME_HPP_

#include "LibrariesExport.h"

#ifdef USE_HAL_DRIVER
#include <HAL_include/HAL.hpp>
#endif

#include <TaskHandler/PriorityTask.hpp>

struct LIBRARIES_EXPORT DateTime
{
private:
#ifdef USE_HAL_DRIVER
	static RTC_HandleTypeDef* hrtc;
#endif

	static uint8_t hours;
	static uint8_t minutes;
	static uint8_t seconds;
	static uint8_t weekday;
	static uint8_t date;
	static uint8_t month;
	static uint8_t year;

public:
#ifdef USE_HAL_DRIVER
    static void Init(RTC_HandleTypeDef* hrtc);
#endif

	static void SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

    static void SetDate(uint8_t date, uint8_t month, uint8_t year);

    static void GetTime(uint8_t& hours, uint8_t& minutes, uint8_t& seconds);

    static void GetDate(uint8_t& date, uint8_t& month, uint8_t& year);

    static void Loop();
};

class LIBRARIES_EXPORT DateTimeServer : public PriorityTask
{
public:
	DateTimeServer(const StaticText32& name, uint32_t period_ms, uint8_t priority)
        : PriorityTask{ name, period_ms, priority }
    { }

	bool Do() override;
};

#endif
