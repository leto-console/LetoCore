/**
 * @file DateTime.hpp
 * @date Mar 19, 2026
 * @author Rakhimov T.
 */

#ifndef INC_TIME_DATE_TIME_HPP_
#define INC_TIME_DATE_TIME_HPP_

#include "LibrariesExport.h"

#ifdef USE_HAL_DRIVER
#include <HAL_include/HAL.hpp>
#else
#include <Time/Timer.hpp>
#endif

#include <cstdint>

#include <Data/IDataCell.hpp>
#include <Time/DateTimeTypes.hpp>

struct LIBRARIES_EXPORT DateTime
{
private:
#ifdef USE_HAL_DRIVER
	static RTC_HandleTypeDef* hrtc;
#else
	static Timer timer_1s;
#endif

	static DateTimeStruct current;

	static IDataCell<DateTimeStruct>* last_exact_datetime;
	static IDataCell<int32_t>* ppm_cell;

public:
#ifdef USE_HAL_DRIVER
    static void Init(RTC_HandleTypeDef* hrtc);
#endif

	/**
	 * Init cells
	 */
	static void InitCells(IDataCell<DateTimeStruct>* _last_exact_datetime, IDataCell<int32_t>* _ppm_cell);

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

#endif
