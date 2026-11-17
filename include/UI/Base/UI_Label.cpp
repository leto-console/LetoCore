#include "UI_Label.hpp"

#include <LetoFunctions/Draw.hpp>
#include <LetoFunctions/Font.hpp>

NEW_LETO_UI_USING

UI_Label::UI_Label(Point2_i size) : UI_Object{ size }
{
    //text_height = DrawFunctions::GetDefaultFont()->GetHeight();
}

bool UI_Label::ObjectOnEvent(const AppEvent &event)
{
    if (IsUIEvent(event, UIE_ON_INVALIDATE))
        UpdateOffsets();

    return false;
}

void UI_Label::ObjectDraw(LetoScreen_V1 *screen)
{
    Point2_i _draw_pos = GetPosition() + Point2_i(h_offset, v_offset);

    LetoColor_V1 text_clr = label_inverse ? label_background_color : label_text_color;
    LetoColor_V1 back_clr = label_inverse ? label_text_color : label_background_color;

    leto::graphics::DrawString(screen, _draw_pos, label_text, label_font, text_clr, back_clr, label_style);

    if (selection_theme) selection_theme->DrawSelection(screen, *this);
}

void UI_Label::SetText(StaticTextView text)
{
    if (text != label_text)
    {
        label_text = text;
        Invalidate();
    }
}

StaticTextView UI_Label::GetText() const
{
    return label_text;
}

void UI_Label::SetHorizonAlignment(UI_Label_HAlign alignment)
{
    if (alignment == h_align) return;

    h_align = alignment;

    Invalidate();
}

void UI_Label::SetVerticalAlignment(UI_Label_VAlign alignment)
{
    if (alignment == v_align) return;

    v_align = alignment;

    Invalidate();
}

void UI_Label::SetFont(const LetoFont_V1 *font)
{
    if (font)
    {
        label_font = font;
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
        Invalidate();
    }
}

LetoTextStyle_V1 UI_Label::GetStyle() const
{
    return label_style;
}

void UI_Label::UpdateOffsets()
{
    text_width = leto::graphics::GetTextWidth(label_text, label_font, label_style);
    text_height = leto::font::GetHeight(label_font);

    h_offset = v_offset = 0;
    
    switch (h_align)
    {
    case UI_Label_HAlign::CENTER:
        h_offset += GetSize().x / 2;
        h_offset -= text_width / 2;
        break;
    case UI_Label_HAlign::RIGHT:
        h_offset = GetSize().x;
        h_offset -= text_width;
        break;
    case UI_Label_HAlign::LEFT:
    default:
        break;
    }

    switch (v_align)
    {
    case UI_Label_VAlign::CENTER:
        v_offset += GetSize().y / 2;
        v_offset -= text_height / 2;
        break;
    case UI_Label_VAlign::BOTTOM:
        v_offset = GetSize().y;
        v_offset -= text_height;
        break;
    case UI_Label_VAlign::TOP:
    default:
        break;
    }
}

void UI_Label::SetSelectionTheme(const UI_SelectionTheme* theme)
{
    if (selection_theme != theme)
    {
        selection_theme = theme;
        Invalidate();
    }
}
