#include "DateTime.hpp"

#ifdef USE_HAL_DRIVER
#include <HAL_include/HAL.hpp>

RTC_HandleTypeDef* DateTime::hrtc = nullptr;

void DateTime::Init(RTC_HandleTypeDef *hrtc)
{
	DateTime::hrtc = hrtc;
}
#else
#include <Time/Timer.hpp>
Timer DateTime::timer_1s{ 1000 };
#endif

#include <System/IndigoScreen.hpp>

DateTimeStruct DateTime::current{};

IDataCell<DateTimeStruct>* DateTime::last_exact_datetime = nullptr;
IDataCell<int32_t>* DateTime::smooth_calib_pulses = nullptr;

void DateTime::InitCells(IDataCell<DateTimeStruct>* _last_exact_datetime, IDataCell<int32_t>* _smooth_calib_pulses)
{
	last_exact_datetime = _last_exact_datetime;
	smooth_calib_pulses = _smooth_calib_pulses;
}

void DateTime::SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	if (hours > 23 ||
		minutes > 59 ||
		seconds > 59)
		return;

	current.time.hours = hours;
	current.time.minutes = minutes;
	current.time.seconds = seconds;

#ifdef USE_HAL_DRIVER
	RTC_TimeTypeDef sTime = {0};

	sTime.Hours = hours;
	sTime.Minutes = minutes;
	sTime.Seconds = seconds;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	if (HAL_RTC_SetTime(hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		CrashIndigoScreen("HAL_RTC_SetTime", "!= HAL_OK");
	}
#else
	timer_1s.Start();
#endif

	if (last_exact_datetime) 
		last_exact_datetime->Set(current);
}

void DateTime::SetDate(uint8_t day, uint8_t month, uint8_t year)
{
	if (day < 1 || 
		day > 31 ||
		// weekday < RTC_WEEKDAY_MONDAY || 
		// weekday > RTC_WEEKDAY_SUNDAY ||
		month < 1 ||
		month > 12 ||
		year > 99)
		return;

	current.date.date 	= day - 1;
	current.date.month 	= month - 1;
	current.date.year 	= year;

#ifdef USE_HAL_DRIVER
	RTC_DateTypeDef sDate = {0};

	sDate.WeekDay 	= RTC_WEEKDAY_MONDAY;
	sDate.Month 	= month;
	sDate.Date 		= day;
	sDate.Year 		= year;

	if (HAL_RTC_SetDate(hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		CrashIndigoScreen("HAL_RTC_SetDate", "!= HAL_OK");
	}
#endif

	if (last_exact_datetime) 
		last_exact_datetime->Set(current);
}

void DateTime::GetTime(uint8_t &hours, uint8_t &minutes, uint8_t &seconds)
{
	hours 	= current.time.hours;
	minutes = current.time.minutes;
	seconds = current.time.seconds;
}

void DateTime::GetDate(uint8_t& day, uint8_t& month, uint8_t& year)
{
	day 	= current.date.date + 1;
	month 	= current.date.month + 1;
	year 	= current.date.year;
}

void DateTime::Loop()
{
#ifdef USE_HAL_DRIVER
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

    HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);

    current.time.hours 		= sTime.Hours;
    current.time.minutes 	= sTime.Minutes;
    current.time.seconds 	= sTime.Seconds;

    current.date.weekday 	= sDate.WeekDay;
    current.date.date 		= sDate.Date - 1;
    current.date.month 		= sDate.Month - 1;
    current.date.year 		= sDate.Year;
#else
	if (timer_1s.Expired())
	{
		timer_1s.Start();

		current.time.seconds++;
		current.time.minutes 	+= (current.time.seconds / 60); 	current.time.seconds %= 60;
		current.time.hours 		+= (current.time.minutes / 60); 	current.time.minutes %= 60;

		current.date.date 		+= (current.time.hours 	/ 24);		current.time.hours 	%= 24;
		current.date.month		+= (current.date.date 	/ 31);		current.date.date 	%= 31;		///< TODO: Слишком грубо
		current.date.year		+= (current.date.month 	/ 12);		current.date.month 	%= 12;
	}

#endif
}
