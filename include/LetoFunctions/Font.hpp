#pragma once

#include <LetoAPI_V1/LetoAPI_V1.h>

namespace leto::font
{
    
    /**
     * @brief Find font
     * 
     * @param[in] height Font height
     * @param[in] type Font type
     */
    inline const LetoFont_V1* FindFont(uint8_t height, LetoFont_V1_Type type)
    {
        return leto_api_v1->Font->FindFont(height, type);
    }

    /**
     * @brief Get height of font
     * 
     * @param[in] font Font instance
     */
    inline uint8_t GetHeight(const LetoFont_V1* font)
    {
        return leto_api_v1->Font->GetHeight(font);
    }

    /**
     * @brief Get type of font
     * 
     * @param[in] font Font instance
     */
    inline LetoFont_V1_Type GetType(const LetoFont_V1* font)
    {
        return leto_api_v1->Font->GetType(font);
    }

}