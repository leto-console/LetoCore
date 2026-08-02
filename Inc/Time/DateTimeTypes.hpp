/**
 * @file DateTimeTypes.hpp
 * @date Jul 26, 2026
 * @author Rakhimov T.
 */

#ifndef INC_TIME_DATE_TIME_TYPES_HPP_
#define INC_TIME_DATE_TIME_TYPES_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <Time/PPM_Info.hpp>

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

#pragma pack(push, 1)

struct LETO_CORE_EXPORT DateTimeStruct
{
	TimeStruct time;
	DateStruct date;

    int32_t DiffSeconds(const DateTimeStruct& other) const;

    uint32_t ToUnixTime() const;
    //void FromUnixTime(uint32_t unix_time);
};

#pragma pack(pop)

#endif
