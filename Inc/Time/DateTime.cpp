#include "DateTime.hpp"

#ifdef USE_HAL_DRIVER
#include <HAL_include/HAL.hpp>

RTC_HandleTypeDef* DateTime::hrtc = nullptr;

void DateTime::Init(RTC_HandleTypeDef *hrtc)
{
	DateTime::hrtc = hrtc;
}
#endif

uint8_t DateTime::hours = 0;
uint8_t DateTime::minutes = 0;
uint8_t DateTime::seconds = 0;
uint8_t DateTime::weekday = 1;
uint8_t DateTime::date = 1;
uint8_t DateTime::month = 1;
uint8_t DateTime::year = 0;

void DateTime::GetTime(uint8_t &hours, uint8_t &minutes, uint8_t &seconds)
{
	hours = DateTime::hours;
	minutes = DateTime::minutes;
	seconds = DateTime::seconds;
}

void DateTime::GetDate(uint8_t& date, uint8_t& month, uint8_t& year)
{
	date = DateTime::date;
	month = DateTime::month;
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
    date = sDate.Date;
    month = sDate.Month;
    year = sDate.Year;
#endif
}

bool DateTimeServer::Do()
{
	DateTime::Loop();
	return true;
}
