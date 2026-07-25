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
#include <Data/IDataCell.hpp>

struct TimeStruct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
};

struct DateStruct
{
	uint8_t weekday;
	uint8_t date;
	uint8_t month;
	uint8_t year;
};

struct LIBRARIES_EXPORT DateTime
{
private:
#ifdef USE_HAL_DRIVER
	static RTC_HandleTypeDef* hrtc;
#endif

	static TimeStruct time;
	static DateStruct date;

	static IDataCell<TimeStruct>* last_exact_time_cell;
	static IDataCell<DateStruct>* last_exact_date_cell;
	static IDataCell<int32_t>* smooth_calib_pulses_cell;

public:
#ifdef USE_HAL_DRIVER
    static void Init(RTC_HandleTypeDef* hrtc);
#endif

	/**
	 * Init cells
	 */
	static void InitCells(
		IDataCell<TimeStruct>* last_exact_time,
		IDataCell<DateStruct>* last_exact_date,
		IDataCell<int32_t>* smooth_calib_pulses
	);

	/**
	 * Set exact time (with auto-correction)
	 */
	static void SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

	/**
	 * Set exact date
	 */
    static void SetDate(uint8_t day, uint8_t month, uint8_t year);

	/**
	 * Get RTC time
	 */
    static void GetTime(uint8_t& hours, uint8_t& minutes, uint8_t& seconds);

	/**
	 * Get RTC date
	 */
    static void GetDate(uint8_t& day, uint8_t& month, uint8_t& year);

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
