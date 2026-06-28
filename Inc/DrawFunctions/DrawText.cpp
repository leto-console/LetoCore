#include "DrawText.hpp"

#include "DrawBitmap.hpp"

#define __GET_SYMBOL(text) (text < 0 ? 256 + text : text)

static const IFont* defaultFont = nullptr;

static bool CheckAndSetFont(const IFont*& font)
{
    if (font) return true;
    if (!defaultFont) return false;
    font = defaultFont;
    return true;
}

void DrawFunctions::SetDefaultFont(const IFont *font)
{
    defaultFont = font;
}

const IFont *DrawFunctions::GetDefaultFont()
{
    return defaultFont;
}

void DrawFunctions::DrawChar(IScreen& screen, Point2_i point, const char *symbol, RGBColor color, RGBColor background, bool inverse, const IFont *font)
{
    if (!CheckAndSetFont(font)) 
        return;

    int symbol_1 = __GET_SYMBOL(symbol[0]);

    if (symbol_1 == 208 || symbol_1 == 209)
    {
        // Русский символ - Wide char
        int symbol_2 = __GET_SYMBOL(symbol[1]);

        DrawFunctions::DrawBitmap(screen, point, font->GetRussianChar(symbol_1, symbol_2), font->GetWidth(), font->GetHeight(), color, background, inverse);
    }
    else if (symbol_1 >= 32 && symbol_1 <= 122)
    {
        DrawFunctions::DrawBitmap(screen, point, font->GetASCIIChar(symbol_1), font->GetWidth(), font->GetHeight(), color, background, inverse);
    }
	else 
	{
		DrawFunctions::DrawBitmap(screen, point, font->GetEmptyChar(), font->GetWidth(), font->GetHeight(), color, background, inverse);
	}
}

void DrawFunctions::DrawText(IScreen &screen, Point2_i point, const char *text, size_t length, RGBColor color, RGBColor background, bool inverse, const IFont *font)
{
    if (!CheckAndSetFont(font) || !text) 
        return;

	size_t idx{};
	int symbol{};
	uint8_t font_width = font->GetWidth(), font_height = font->GetHeight();

	while (text[idx] != '\0' && idx < length) {
		symbol = __GET_SYMBOL(text[idx]);

		if (symbol == 208 || symbol == 209)
		{
			// Русский символ - Wide char
			int first_symbol = symbol;

			++idx;
			symbol = __GET_SYMBOL(text[idx]);

			DrawFunctions::DrawBitmap(screen, point, font->GetRussianChar(first_symbol, symbol), font_width, font_height, color, background, inverse);
		}
		else if (symbol >= 32 && symbol <= 122)
		{
			DrawFunctions::DrawBitmap(screen, point, font->GetASCIIChar(symbol), font_width, font_height, color, background, inverse);
		}
		else
		{
			DrawFunctions::DrawBitmap(screen, point, font->GetEmptyChar(), font_width, font_height, color, background, inverse);
		}
		++idx;
		point.x += font_width;
	}

}

int DrawFunctions::TextWidth(const char *text, size_t length, const IFont *font)
{
    if (!CheckAndSetFont(font) || !text) 
        return 0;

	int width = 0;

	size_t idx{};
	int symbol{};
	uint8_t font_width = font->GetWidth();

	while (text[idx] != '\0' && idx < length) {
		symbol = __GET_SYMBOL(text[idx]);
		if (symbol == 208 || symbol == 209)
		{
			++idx;
		}
		++idx;
		width += font_width;
	}

	return width;
}
