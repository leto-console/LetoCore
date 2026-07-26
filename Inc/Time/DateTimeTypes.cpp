#include "DateTimeTypes.hpp"

static const uint32_t days_passed_on_month[]
{
	0,
	31,
	31 + 28,
	31 + 28 + 31,
	31 + 28 + 31 + 30,
	31 + 28 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30
};

constexpr bool IsBissextus(int year)
{
	return year % 400 == 0 ? true : year % 100 == 0 ? false : year % 4 == 0;
}

constexpr uint32_t MIN_SEC = 60;
constexpr uint32_t HOUR_SEC = 60 * MIN_SEC;
constexpr uint32_t DAY_SEC = 24 * HOUR_SEC;

uint32_t DateTimeStruct::ToUnixTime() const
{
	uint32_t res = time.seconds;
	res += time.minutes * MIN_SEC;
	res += time.hours * HOUR_SEC;

	int year = (date.year > 70 ? 1900 : 2000) + date.year;

	int days = date.date + days_passed_on_month[date.month] + IsBissextus(year) * bool(date.month);
	res += days * DAY_SEC;

	while (--year >= 1970)
		res += DAY_SEC * (365 + IsBissextus(year));

	return res;
}

//void DateTimeStruct::FromUnixTime(uint32_t unix_time)
//{
//    int year = 1970;
//
//    int sec_in_year = DAY_SEC * (365 + IsBissextus(year));
//
//    while (unix_time > sec_in_year)
//    {
//        unix_time -= sec_in_year;
//        sec_in_year = DAY_SEC * (365 + IsBissextus(++year));
//    }
//
//    
//    date.year = year % 100;
//}
