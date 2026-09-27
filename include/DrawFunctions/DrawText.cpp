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
	
	const char CTRL_OPEN_SYMBOL = '{';
	const char CTRL_CLOSE_SYMBOL = '}';

	RGBColor draw_color = color; 
	size_t idx{};
	int symbol{};
	uint8_t font_width = font->GetWidth(), font_height = font->GetHeight();

	while (text[idx] != '\0' && idx < length) {
		symbol = __GET_SYMBOL(text[idx]);

		if (symbol == CTRL_OPEN_SYMBOL) 
		{ 
			// Обработка тегов управления
			idx += 2;
			symbol = __GET_SYMBOL(text[idx]);

			/* 
				{ #RRGGBB } - смена цвета
				{ # } - отмена смены цвета
			*/
			if (symbol == '#') 
			{
				if (idx + 2 < length)
				{
					symbol = __GET_SYMBOL(text[idx + 2]);
					if (symbol == CTRL_CLOSE_SYMBOL)			/* { # } - отмена смены цвета */
					{
						draw_color = color;
						idx += 3;
						continue;
					}
				}
				if (idx + 8 < length)
				{
					symbol = __GET_SYMBOL(text[idx + 8]);		/* { #RRGGBB } - смена цвета */
					if (symbol == CTRL_CLOSE_SYMBOL)
					{
						draw_color = RGBColor{ &text[idx] };
						idx += 9;
						continue;
					}
				}
			}

			++idx;
			continue;
		}
		else if (symbol == 208 || symbol == 209)
		{
			// Русский символ - Wide char
			int first_symbol = symbol;

			++idx;
			symbol = __GET_SYMBOL(text[idx]);

			DrawFunctions::DrawBitmap(screen, point, font->GetRussianChar(first_symbol, symbol), font_width, font_height, draw_color, background, inverse);
		}
		else if (symbol >= 32 && symbol <= 122)
		{
			DrawFunctions::DrawBitmap(screen, point, font->GetASCIIChar(symbol), font_width, font_height, draw_color, background, inverse);
		}
		else
		{
			DrawFunctions::DrawBitmap(screen, point, font->GetEmptyChar(), font_width, font_height, draw_color, background, inverse);
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

// ===== GlyphFont =====

void DrawFunctions::DrawGlyph(IScreen &screen, Point2_i point, const GlyphData &data, RGBColor bitmap_color, RGBColor background_color, bool inverse)
{
	DrawFunctions::DrawBitmap(screen, point, data.bitmap, data.width, data.height, bitmap_color, background_color, inverse);
}

void DrawFunctions::DrawChar(IScreen &screen, Point2_i point, const char *symbol, const GlyphFont *font, RGBColor color, RGBColor background, bool inverse)
{
    if (!font) 
        return;

    int symbol_1 = __GET_SYMBOL(symbol[0]);

    if (symbol_1 == 208 || symbol_1 == 209)
    {
        // Русский символ - Wide char
        int symbol_2 = __GET_SYMBOL(symbol[1]);

        DrawFunctions::DrawGlyph(screen, point, font->GetUTF8(symbol_1, symbol_2), color, background, inverse);
    }
    else if (symbol_1 >= 32 && symbol_1 <= 122)
    {
		DrawFunctions::DrawGlyph(screen, point, font->GetUTF8(symbol_1), color, background, inverse);
    }
	else 
	{
		DrawFunctions::DrawGlyph(screen, point, font->GetUTF8(__GET_SYMBOL(' ')), color, background, inverse);
	}
}

#define SYMBOL_INTERVAL 2 // TODO:: Вынести в отдельный аргумент

void DrawFunctions::DrawText(IScreen &screen, Point2_i point, const char *text, size_t length, const GlyphFont *font, RGBColor color, RGBColor background, bool inverse)
{
    if (!font || !text) 
        return;
	
	const char CTRL_OPEN_SYMBOL = '{';
	const char CTRL_CLOSE_SYMBOL = '}';

	RGBColor draw_color = color; 
	size_t idx{};
	int symbol{};

	while (text[idx] != '\0' && idx < length) {
		symbol = __GET_SYMBOL(text[idx]);

		if (symbol == CTRL_OPEN_SYMBOL) 
		{ 
			// Обработка тегов управления
			idx += 2;
			symbol = __GET_SYMBOL(text[idx]);

			/* 
				{ #RRGGBB } - смена цвета
				{ # } - отмена смены цвета
			*/
			if (symbol == '#') 
			{
				if (idx + 2 < length)
				{
					symbol = __GET_SYMBOL(text[idx + 2]);
					if (symbol == CTRL_CLOSE_SYMBOL)			/* { # } - отмена смены цвета */
					{
						draw_color = color;
						idx += 3;
						continue;
					}
				}
				if (idx + 8 < length)
				{
					symbol = __GET_SYMBOL(text[idx + 8]);		/* { #RRGGBB } - смена цвета */
					if (symbol == CTRL_CLOSE_SYMBOL)
					{
						draw_color = RGBColor{ &text[idx] };
						idx += 9;
						continue;
					}
				}
			}

			++idx;
			continue;
		}
		else if (symbol == 208 || symbol == 209)
		{
			// Русский символ - Wide char
			int first_symbol = symbol;

			++idx;
			symbol = __GET_SYMBOL(text[idx]);
			
			GlyphData data = font->GetUTF8(first_symbol, symbol);
			DrawFunctions::DrawGlyph(screen, point, data, draw_color, background, inverse);
			point.x += (data.width + SYMBOL_INTERVAL); // TODO: межсимвольный интервал
		}
		else if (symbol >= 32 && symbol <= 122)
		{
			GlyphData data = font->GetUTF8(symbol);
			DrawFunctions::DrawGlyph(screen, point, font->GetUTF8(symbol), draw_color, background, inverse);
			point.x += (data.width + SYMBOL_INTERVAL); // TODO: межсимвольный интервал
		}
		else
		{
			GlyphData data = font->GetUTF8(__GET_SYMBOL(' '));
			DrawFunctions::DrawGlyph(screen, point, font->GetUTF8(__GET_SYMBOL(' ')), draw_color, background, inverse);
			point.x += (data.width + SYMBOL_INTERVAL); // TODO: межсимвольный интервал
		}
		++idx;
	}
}

int DrawFunctions::TextWidth(const char *text, size_t length, const GlyphFont *font)
{
    if (!font || !text) 
        return 0;

	int width = 0;

	size_t idx{};
	int symbol{};

	while (text[idx] != '\0' && idx < length) {
		symbol = __GET_SYMBOL(text[idx]);
		if (symbol == 208 || symbol == 209)
		{
			++idx;
			width += font->GetUTF8(symbol, __GET_SYMBOL(text[idx])).width;
			width += SYMBOL_INTERVAL; // TODO: межсимвольный интервал
		}
		else
		{
			width += font->GetUTF8(symbol).width;
			width += SYMBOL_INTERVAL; // TODO: межсимвольный интервал
		}
		++idx;
	}

	return width;
}
