/**
 * @file DrawText.hpp
 * @date Mar 21, 2026
 * @author Rakhimov T.
 */

#pragma once

#include "LetoCoreExport.h"

#include <Graphics/IScreen.hpp>
#include <Graphics/RGBColor.hpp>
#include <Data/StaticTextView.hpp> 

#define TXT_COLOR_OPEN(txt) "{" txt "}"
#define TXT_COLOR_CLOSE "{ # }"

namespace DrawFunctions
{
    extern LETO_CORE_EXPORT void SetDefaultFont(const IFont* font);

    extern LETO_CORE_EXPORT const IFont* GetDefaultFont();

    // Отрисовать символ в кодировке UTF-8
    extern LETO_CORE_EXPORT void DrawChar(IScreen& screen, Point2_i point, const char* symbol, RGBColor color = WhiteColor, RGBColor background = BlackColor, bool inverse = false, const IFont* font = nullptr);

    // Отрисовать текст в кодировке UTF-8
    extern LETO_CORE_EXPORT void DrawString(IScreen& screen, Point2_i point, const char* text, size_t length, RGBColor color = WhiteColor, RGBColor background = BlackColor, bool inverse = false, const IFont* font = nullptr);

    inline void DrawString(IScreen& screen, Point2_i point, StaticTextView text_view, RGBColor color = WhiteColor, RGBColor background = BlackColor, bool inverse = false, const IFont* font = nullptr)
	{
		DrawString(screen, point, text_view.ConstChar(), text_view.Capacity(), color, background, inverse, font);
	}

    // Получить ширину текста на экране
    extern LETO_CORE_EXPORT int TextWidth(const char* text, size_t length, const IFont* font = nullptr);

    inline int TextWidth(StaticTextView text_view, const IFont* font = nullptr)
	{
		return TextWidth(text_view.ConstChar(), text_view.Capacity(), font);
	}

}
