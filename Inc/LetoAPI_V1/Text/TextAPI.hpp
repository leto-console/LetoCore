/**
 * @file TextAPI_V1.hpp
 * @date May 23, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_TEXT_TEXT_API_V1_HPP_
#define INC_LETO_API_V1_TEXT_TEXT_API_V1_HPP_

#include <cstdint>
#include <cstring>

#include <type_traits>

// Align to 4-byte boundary
#pragma pack(push, 4)

/// Text processing functions
struct TextAPI_V1
{
    /// Format text string
    int (*FormatText)(char* const buffer, const size_t buffer_size, const char* const format, ...);

    /// Format floating-point value
    int (*FormatFloat)(char* const buffer, const size_t buffer_size, const size_t fract_part, float value);

    /// Scan and parse text string
    int (*ScanText)(char* const buffer, const size_t buffer_size, const char* const format, ...);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<TextAPI_V1>::value, "TextAPI_V1 must be a standard layout type");

#endif
