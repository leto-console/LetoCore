#pragma once

#include <LetoAPI_V1/LetoAPI_V1.h>

namespace leto::graphics
{
    
    /**
     * @brief Draw a UTF-8 text
     * 
     * @param[in] screen Target screen instance
     * @param[in] x Top-left X coordinate
     * @param[in] y Top-left Y coordinate
     * @param[in] text Null-terminated text pointer
     * @param[in] length Count of bytes in the text
     * @param[in] font Text font
     * @param[in] color Text color
     * @param[in] background Background color
     * @param[in] style Text style
     */
    inline void DrawText(
        LetoScreen_V1* screen, int32_t x, int32_t y, 
        const char* text, uint32_t length, 
        const LetoFont_V1* font = nullptr, 
        LetoColor_V1 color = WhiteColor, LetoColor_V1 background = BlackColor,
        LetoTextStyle_V1 style = {2,2})
    {
        leto_api_v1->Graphics->DrawText(screen, x, y, text, length, font, color, background, style);
    }

    /**
     * @brief Get a UTF-8 text width on screen
     * 
     * @param[in] text Null-terminated text pointer
     * @param[in] length Count of bytes in the text
     * @param[in] font Text font
     * @param[in] style Text style
     */
    inline uint32_t GetTextWidth(const char* text, uint32_t length, const LetoFont_V1* font, LetoTextStyle_V1 style)
    {
        return leto_api_v1->Graphics->GetTextWidth(text, length, font, style);
    }

    inline void DrawText(
        LetoScreen_V1* screen, Point2_i point, 
        StaticTextView text_view,
        const LetoFont_V1* font = nullptr, 
        LetoColor_V1 color = WhiteColor, LetoColor_V1 background = BlackColor,
        LetoTextStyle_V1 style = {2,2})
	{
		DrawText(screen, point.x, point.y, text_view.ConstChar(), text_view.Capacity(), font, color, background, style);
	}

    inline uint32_t GetTextWidth(StaticTextView text_view, const LetoFont_V1* font, LetoTextStyle_V1 style = {2,2})
    {
        return GetTextWidth(text_view.ConstChar(), text_view.Capacity(), font, style);
    }

}
