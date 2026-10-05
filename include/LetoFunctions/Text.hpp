#pragma once

#include <LetoAPI_V1/LetoAPI_V1.h>
#include <utility>

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

}