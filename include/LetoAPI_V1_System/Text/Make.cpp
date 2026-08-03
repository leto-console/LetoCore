#include "Make.hpp"

#include <cstdio>
#include <cstdarg>

// ====================================================================================================

static int FormatText(
    char* const buffer, 
    const size_t buffer_size, 
    const char* const format, 
    ...)
{
	va_list args;
	va_start(args, format);
	int res = vsnprintf(buffer, buffer_size, format, args);
	va_end(args);
	return res;
}

static int FormatFloat(
    char* const buffer, 
    const size_t buffer_size, 
    const size_t fract_part, 
    float value)
{
	if (fract_part == 0)
	{
		long whole = static_cast<long>(value + 0.5f);
		return snprintf(buffer, buffer_size, "%d", whole);
	}

	long whole = static_cast<long>(value);

	value -= whole;
	for (size_t i = 0; i < fract_part; ++i)
		value *= 10.f;
	value += 0.5f;

	long fract = static_cast<long>(value);

	return snprintf(buffer, buffer_size, "%d.%0*d", whole, static_cast<int>(fract_part), fract);
}

// ====================================================================================================

const TextAPI_V1* Make_TextAPI()
{
	static const TextAPI_V1 api
	{
		&FormatText,
		&FormatFloat
	};

    return &api;
}
