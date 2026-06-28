/*
 * DrawText.hpp
 *
 *  Created on: Mar 21, 2026
 *      Author: Timur
 */

#ifndef INC_DRAW_FUNCTIONS_DRAW_TEXT_HPP_
#define INC_DRAW_FUNCTIONS_DRAW_TEXT_HPP_

#include "LibrariesExport.h"

#include <Graphics/IScreen.hpp>
#include <Graphics/RGBColor.hpp>
#include <Data/StaticTextView.hpp>

namespace DrawFunctions
{
    extern LIBRARIES_EXPORT void SetDefaultFont(const IFont* font);

    extern LIBRARIES_EXPORT const IFont* GetDefaultFont();

    // Отрисовать символ в кодировке UTF-8
    extern LIBRARIES_EXPORT void DrawChar(IScreen& screen, Point2_i point, const char* symbol, RGBColor color = WhiteColor, RGBColor background = BlackColor, bool inverse = false, const IFont* font = nullptr);

    // Отрисовать текст в кодировке UTF-8
    extern LIBRARIES_EXPORT void DrawText(IScreen& screen, Point2_i point, const char* text, size_t length, RGBColor color = WhiteColor, RGBColor background = BlackColor, bool inverse = false, const IFont* font = nullptr);

    inline void DrawText(IScreen& screen, Point2_i point, StaticTextView text_view, RGBColor color = WhiteColor, RGBColor background = BlackColor, bool inverse = false, const IFont* font = nullptr)
	{
		DrawText(screen, point, text_view.ConstChar(), text_view.Capacity(), color, background, inverse, font);
	}

    // Получить ширину текста на экране
    extern LIBRARIES_EXPORT int TextWidth(const char* text, size_t length, const IFont* font = nullptr);

    inline int TextWidth(StaticTextView text_view, const IFont* font = nullptr)
	{
		return TextWidth(text_view.ConstChar(), text_view.Capacity(), font);
	}

}

#endif