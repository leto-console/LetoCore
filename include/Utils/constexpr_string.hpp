/**
 * @file constexpr_string.hpp
 * @date Oct 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UTILS_CONSTEXPR_STRING_HPP_
#define INC_UTILS_CONSTEXPR_STRING_HPP_

#include <cstddef>

namespace constexpr_string
{
    // Аналог strlen (считает до первого нуль-терминатора)
    constexpr size_t strlen(const char* str) noexcept
    {
        if (!str) return 0;
        size_t len = 0;
        while (str[len] != '\0')
        {
            ++len;
        }
        return len;
    }

    // Аналог strnlen
    constexpr size_t strnlen(const char* str, size_t max_len) noexcept
    {
        if (!str) return 0;
        size_t len = 0;
        while (len < max_len && str[len] != '\0')
        {
            ++len;
        }
        return len;
    }

    // Аналог memcpy (возвращает количество скопированных байт)
    constexpr size_t memcpy(char* dest, const char* src, size_t count) noexcept
    {
        if (!dest || !src) return 0;
        for (size_t i = 0; i < count; ++i)
        {
            dest[i] = src[i];
        }
        return count;
    }

    // Аналог strncmp (возвращает 0, если строки равны на длину max_len)
    constexpr int strncmp(const char* s1, const char* s2, size_t max_len) noexcept
    {
        if (!s1 || !s2) return (s1 == s2) ? 0 : (s1 ? 1 : -1);
        for (size_t i = 0; i < max_len; ++i)
        {
            if (s1[i] != s2[i])
            {
                return (static_cast<unsigned char>(s1[i]) < static_cast<unsigned char>(s2[i])) ? -1 : 1;
            }
            if (s1[i] == '\0') return 0;
        }
        return 0;
    }
}

#endif
