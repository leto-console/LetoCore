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

uint8_t DateTime::hours = 0;
uint8_t DateTime::minutes = 0;
uint8_t DateTime::seconds = 0;
uint8_t DateTime::weekday = 1;
uint8_t DateTime::date = 0;
uint8_t DateTime::month = 0;
uint8_t DateTime::year = 0;

void DateTime::SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	if (hours > 23 ||
		minutes > 59 ||
		seconds > 59)
		return;

	DateTime::hours = hours;
	DateTime::minutes = minutes;
	DateTime::seconds = seconds;

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
}

void DateTime::SetDate(uint8_t date, uint8_t month, uint8_t year)
{
	if (date < 1 || 
		date > 31 ||
		// weekday < RTC_WEEKDAY_MONDAY || 
		// weekday > RTC_WEEKDAY_SUNDAY ||
		month < 1 ||
		month > 12 ||
		year > 99)
		return;

	DateTime::date = date - 1;
	DateTime::month = month - 1;
	DateTime::year = year;

#ifdef USE_HAL_DRIVER
	RTC_DateTypeDef sDate = {0};

	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	sDate.Month = month;
	sDate.Date = date;
	sDate.Year = year;

	if (HAL_RTC_SetDate(hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		CrashIndigoScreen("HAL_RTC_SetDate", "!= HAL_OK");
	}
#endif
}

void DateTime::GetTime(uint8_t &hours, uint8_t &minutes, uint8_t &seconds)
{
	hours = DateTime::hours;
	minutes = DateTime::minutes;
	seconds = DateTime::seconds;
}

void DateTime::GetDate(uint8_t& date, uint8_t& month, uint8_t& year)
{
	date = DateTime::date + 1;
	month = DateTime::month + 1;
	year = DateTime::year;
}

void DateTime::Loop()
{
#ifdef USE_HAL_DRIVER
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

    HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);

    hours = sTime.Hours;
    minutes = sTime.Minutes;
    seconds = sTime.Seconds;

    weekday = sDate.WeekDay;
    date = sDate.Date - 1;
    month = sDate.Month - 1;
    year = sDate.Year;
#else
	static Timer timer(1000);

	if (timer.Expired())
	{
		timer.Start();

		seconds++;
		minutes += (seconds / 60); 	seconds %= 60;
		hours 	+= (minutes / 60); 	minutes %= 60;

		date 	+= (hours 	/ 24);	hours 	%= 24;
		month	+= (date 	/ 31);	date 	%= 31;		///< TODO: Слишком грубо
		year	+= (month 	/ 12);	month 	%= 12;
	}

#endif
}

bool DateTimeServer::Do()
{
	DateTime::Loop();
	return true;
}
