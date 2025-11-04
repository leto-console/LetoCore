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

// Выравнивание по границе 4 байт 
#pragma pack(push, 4)

/// Функции для работы с текстом
struct TextAPI_V1
{
    /// Отформатировать текст
    int (*FormatText)(char* const buffer, const size_t buffer_size, const char* const format, ...);

    /// Отформатировать вещественное значение
    int (*FormatFloat)(char* const buffer, const size_t buffer_size, const size_t fract_part, float value);

    /// Сканировать текст
    int (*ScanText)(char* const buffer, const size_t buffer_size, const char* const format, ...);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<TextAPI_V1>::value, "TextAPI_V1 должен быть standard_layout");

#endif
