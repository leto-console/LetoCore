#include "UI_Label.hpp"

#include <LetoFunctions/Draw.hpp>
#include <LetoFunctions/Font.hpp>

#include <UI/Base/UI_Themes.hpp>

NEW_LETO_UI_USING

void UI_Label::ObjectDraw(LetoScreen_V1 *screen)
{
    Point2_i _draw_pos = GetPosition() + align_offset;

    LetoColor_V1 text_clr = label_inverse ? label_background_color : label_text_color;
    LetoColor_V1 back_clr = label_inverse ? label_text_color : label_background_color;

    leto::graphics::DrawString(screen, _draw_pos, label_text, label_font, text_clr, back_clr, label_style);

    if (BaseDrawSelection(screen, *this)) return;
    if (BaseDrawHover(screen, *this)) return;
}

void UI_Label::RefreshAlign()
{
	text_width = leto::graphics::GetTextWidth(label_text, label_font, label_style);
    text_height = leto::font::GetHeight(label_font);
	
	Rect_i text_rect{ align_offset, { text_width, text_height } };
	Point2_i new_offset = AlignRect(text_rect);
	
	if (new_offset != align_offset)
	{
		text_rect.pos += GetPosition();
		InvalidateRect(text_rect);
	}
	
	text_rect.pos = align_offset = new_offset;
	text_rect.pos += GetPosition();
	InvalidateRect(text_rect);
}

void UI_Label::Setup(StaticTextView text, const LetoFont_V1 *font, LetoColor_V1 text_color, LetoColor_V1 back_color, LetoTextStyle_V1 style)
{
    SetText(text);
    SetFont(font);
    SetTextColor(text_color);
    SetBackroundColor(back_color);
    SetStyle(style);
}

void UI_Label::SetText(StaticTextView text)
{
    if (text != label_text)
    {
        label_text = text;
        RefreshAlign();
        Invalidate();
    }
}

StaticTextView UI_Label::GetText() const
{
    return label_text;
}

void UI_Label::SetFont(const LetoFont_V1 *font)
{
    if (font)
    {
        label_font = font;
        RefreshAlign();
        Invalidate();
    }
}

const LetoFont_V1 *UI_Label::GetFont() const
{
    return label_font;
}

void UI_Label::SetTextColor(LetoColor_V1 color)
{
    if (label_text_color != color)
    {
        label_text_color = color;
        Invalidate();
    }
}

LetoColor_V1 UI_Label::GetTextColor() const
{
    return label_text_color;
}

void UI_Label::SetBackroundColor(LetoColor_V1 color)
{
    if (label_background_color != color)
    {
        label_background_color = color;
        Invalidate();
    }
}

LetoColor_V1 UI_Label::GetBackroundColor() const
{
    return label_background_color;
}

void UI_Label::SetColorInverse(bool inverse)
{
    if (label_inverse != inverse)
    {
        label_inverse = inverse;
        Invalidate();
    }
}

void UI_Label::SetStyle(LetoTextStyle_V1 style)
{
    if (memcmp(&label_style, &style, sizeof(LetoTextStyle_V1)) != 0)
    {
        label_style = style;
        RefreshAlign();
        Invalidate();
    }
}

LetoTextStyle_V1 UI_Label::GetStyle() const
{
    return label_style;
}
