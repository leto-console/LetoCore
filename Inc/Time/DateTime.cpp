#include "DateTime.hpp"

#ifdef USE_HAL_DRIVER
#include <HAL_include/HAL.hpp>

extern void Error_Handler(void);

RTC_HandleTypeDef* DateTime::hrtc = nullptr;

void DateTime::Init(RTC_HandleTypeDef *hrtc)
{
	DateTime::hrtc = hrtc;
}
#else
#include <Time/Timer.hpp>
#endif

#include <System/IndigoScreen.hpp>

TimeStruct DateTime::time{};
DateStruct DateTime::date{};

IDataCell<TimeStruct>* DateTime::last_exact_time_cell = nullptr;
IDataCell<DateStruct>* DateTime::last_exact_date_cell = nullptr;
IDataCell<int32_t>* DateTime::smooth_calib_pulses_cell = nullptr;

void DateTime::InitCells(IDataCell<TimeStruct>* last_exact_time, IDataCell<DateStruct>* last_exact_date, IDataCell<int32_t>* smooth_calib_pulses)
{
	last_exact_time_cell = last_exact_time;
	last_exact_date_cell = last_exact_date;
	smooth_calib_pulses_cell = smooth_calib_pulses;
}

void DateTime::SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	if (hours > 23 ||
		minutes > 59 ||
		seconds > 59)
		return;

	DateTime::time.hours = hours;
	DateTime::time.minutes = minutes;
	DateTime::time.seconds = seconds;

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
#endif

	if (last_exact_time_cell) 
		last_exact_time_cell->Set(time);
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

	DateTime::date.date 	= day - 1;
	DateTime::date.month 	= month - 1;
	DateTime::date.year 	= year;

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

	if (last_exact_date_cell) 
		last_exact_date_cell->Set(DateTime::date);
}

void DateTime::GetTime(uint8_t &hours, uint8_t &minutes, uint8_t &seconds)
{
	hours 	= DateTime::time.hours;
	minutes = DateTime::time.minutes;
	seconds = DateTime::time.seconds;
}

void DateTime::GetDate(uint8_t& day, uint8_t& month, uint8_t& year)
{
	day 	= DateTime::date.date + 1;
	month 	= DateTime::date.month + 1;
	year 	= DateTime::date.year;
}

void DateTime::Loop()
{
#ifdef USE_HAL_DRIVER
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

    HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);

    time.hours 		= sTime.Hours;
    time.minutes 	= sTime.Minutes;
    time.seconds 	= sTime.Seconds;

    date.weekday 	= sDate.WeekDay;
    date.date 		= sDate.Date - 1;
    date.month 		= sDate.Month - 1;
    date.year 		= sDate.Year;
#else
	static Timer timer(1000);

	if (timer.Expired())
	{
		timer.Start();

		time.seconds++;
		time.minutes 	+= (time.seconds / 60); 	time.seconds %= 60;
		time.hours 		+= (time.minutes / 60); 	time.minutes %= 60;

		date.date 		+= (time.hours 	/ 24);		time.hours 	%= 24;
		date.month		+= (date.date 	/ 31);		date.date 	%= 31;		///< TODO: Слишком грубо
		date.year		+= (date.month 	/ 12);		date.month 	%= 12;
	}

#endif
}

bool DateTimeServer::Do()
{
	DateTime::Loop();
	return true;
}
