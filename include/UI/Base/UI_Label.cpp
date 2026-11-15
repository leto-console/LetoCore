#include "UI_Label.hpp"

#include <LetoFunctions/Draw.hpp>
#include <LetoFunctions/Font.hpp>

UI_Label_new::UI_Label_new(Point2_i size) : UI_Object{ size }
{
    //text_height = DrawFunctions::GetDefaultFont()->GetHeight();
}

bool UI_Label_new::ObjectOnEvent(const AppEvent &event)
{
    if (IsUIEvent(event, UIE_ON_INVALIDATE))
        UpdateOffsets();

    return false;
}

void UI_Label_new::ObjectDraw(LetoScreen_V1 *screen)
{
    Point2_i _draw_pos = GetPosition() + Point2_i(h_offset, v_offset);

    LetoColor_V1 text_clr = label_inverse ? label_background_color : label_text_color;
    LetoColor_V1 back_clr = label_inverse ? label_text_color : label_background_color;

    leto::graphics::DrawString(screen, _draw_pos, label_text, label_font, text_clr, back_clr, label_style);
}

void UI_Label_new::SetText(StaticTextView text)
{
    if (text != label_text)
    {
        label_text = text;
        Invalidate();
    }
}

StaticTextView UI_Label_new::GetText() const
{
    return label_text;
}

UI_Label_new& UI_Label_new::SetHorizonAlignment(UI_Label_HAlign alignment)
{
    if (alignment == h_align) return *this;

    h_align = alignment;

    Invalidate();
    return *this;
}

UI_Label_new& UI_Label_new::SetVerticalAlignment(UI_Label_VAlign alignment)
{
    if (alignment == v_align) return *this;

    v_align = alignment;

    Invalidate();
    return *this;
}

UI_Label_new& UI_Label_new::SetFont(const LetoFont_V1 *font)
{
    if (font)
    {
        label_font = font;
        Invalidate();
    }
    return *this;
}

const LetoFont_V1 *UI_Label_new::GetFont() const
{
    return label_font;
}

UI_Label_new& UI_Label_new::SetTextColor(LetoColor_V1 color)
{
    if (label_text_color != color)
    {
        label_text_color = color;
        Invalidate();
    }
    return *this;
}

LetoColor_V1 UI_Label_new::GetTextColor() const
{
    return label_text_color;
}

UI_Label_new& UI_Label_new::SetBackroundColor(LetoColor_V1 color)
{
    if (label_background_color != color)
    {
        label_background_color = color;
        Invalidate();
    }
    return *this;
}

LetoColor_V1 UI_Label_new::GetBackroundColor() const
{
    return label_background_color;
}

UI_Label_new& UI_Label_new::SetColorInverse(bool inverse)
{
    if (label_inverse != inverse)
    {
        label_inverse = inverse;
        Invalidate();
    }
    return *this;
}

UI_Label_new &UI_Label_new::SetStyle(LetoTextStyle_V1 style)
{
    if (memcmp(&label_style, &style, sizeof(LetoTextStyle_V1)) != 0)
    {
        label_style = style;
        Invalidate();
    }
    return *this;
}

void UI_Label_new::UpdateOffsets()
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
