#pragma once

#include <LetoAPI_V1/LetoAPI_V1.h>
#include <utility>

#include <Data/StaticText.hpp>

namespace leto::text
{
    
    /**
     * @brief Format text string
     */
    template <typename... Args>
    inline int FormatText(char* const buffer, const size_t buffer_size, const char* const format, Args&&... args)
    {
        return leto_api_v1->Text->FormatText(buffer, buffer_size, format, std::forward<Args>(args)...);
    }

    /**
     * @brief Format text string
     */
    template <size_t TextCapacity, typename... Args>
    inline int FormatText(StaticText<TextCapacity>& text, const char* const format, Args&&... args)
    {
        return leto_api_v1->Text->FormatText(text.CharPtr(), text.Capacity(), format, std::forward<Args>(args)...);
    }

    /**
     * @brief Format float
     */
    inline int FormatFloat(char* const buffer, const size_t buffer_size, const size_t fract_part, float value)
    {
        return leto_api_v1->Text->FormatFloat(buffer, buffer_size, fract_part, value);
    }
    
    /**
     * @brief Format float
     */
    template <size_t TextCapacity>
    inline int FormatFloat(StaticText<TextCapacity>& text, const size_t fract_part, float value)
    {
        return leto_api_v1->Text->FormatFloat(text.CharPtr(), text.Capacity(), fract_part, value);
    }
}