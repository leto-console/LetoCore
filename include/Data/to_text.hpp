/**
 * @file to_text.hpp
 * @date Oct 08, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_TO_TEXT_HPP_
#define INC_DATA_TO_TEXT_HPP_

#include <Data/StaticText.hpp>
#include <LetoFunctions/Text.hpp>

template <typename T>
inline StaticText32 to_text(T value, const char* fmt)
{
    StaticText32 text;
    leto::text::FormatText(text, fmt, value);
    return text;
}

inline StaticText32 to_text(float value, const char* fmt)
{
    StaticText32 text;
    leto::text::FormatFloat(text, 4, value);
    return text;
}

inline StaticText32 to_text(int value)                  { return to_text(value, "%d"); }
inline StaticText32 to_text(long value)                 { return to_text(value, "%ld"); }
inline StaticText32 to_text(long long value)            { return to_text(value, "%lld"); }
inline StaticText32 to_text(unsigned value)             { return to_text(value, "%u"); }
inline StaticText32 to_text(unsigned long value)        { return to_text(value, "%lu"); }
inline StaticText32 to_text(unsigned long long value)   { return to_text(value, "%llu"); }
inline StaticText32 to_text(float value)                { return to_text(value, "%f"); }

#endif