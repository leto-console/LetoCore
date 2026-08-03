#include "UI_Label.hpp"

#include <DrawFunctions/DrawText.hpp>

UI_Label::UI_Label(Point2_i pos, Point2_i size)
{
    SetPosition(pos);
    SetWidth(size.x);
    SetHeight(size.y);

    text_height = DrawFunctions::GetDefaultFont()->GetHeight();
}

void UI_Label::Draw(IScreen &screen, Point2_i offset)
{
    using namespace DrawFunctions;

    Point2_i _draw_pos = position + offset + Point2_i(h_offset, v_offset);
    
    DrawText(screen, _draw_pos, label_text, label_text_color, label_background_color, label_inverse, label_font);
}

bool UI_Label::ProcessInput(const AppEvent &event)
{
    (void) event;
    return false;
}

void UI_Label::SetText(const StaticText32 &text)
{
    if (text != label_text)
    {
        label_text = text;
        text_width = DrawFunctions::TextWidth(label_text, label_font);
        UpdateOffsets();
        Invalidate();
    }
}

StaticTextView UI_Label::GetText() const
{
    return label_text;
}

UI_Label& UI_Label::SetHorizonAlignment(LabelHorizonAlignment alignment)
{
    if (alignment == h_align) return *this;

    h_align = alignment;

    UpdateOffsets();
    Invalidate();
    return *this;
}

UI_Label& UI_Label::SetVerticalAlignment(LabelVerticalAlignment alignment)
{
    if (alignment == v_align) return *this;

    v_align = alignment;

    UpdateOffsets();
    Invalidate();
    return *this;
}

UI_Label& UI_Label::SetFont(const IFont *font)
{
    if (font)
    {
        label_font = font;
        text_width = DrawFunctions::TextWidth(label_text, label_font);
        text_height = label_font->GetHeight();
        UpdateOffsets();
        Invalidate();
    }
    return *this;
}

UI_Label& UI_Label::SetTextColor(RGBColor color)
{
    if (label_text_color != color)
    {
        label_text_color = color;
        Invalidate();
    }
    return *this;
}

RGBColor UI_Label::GetTextColor() const
{
    return label_text_color;
}

UI_Label& UI_Label::SetBackroundColor(RGBColor color)
{
    if (label_background_color != color)
    {
        label_background_color = color;
        Invalidate();
    }
    return *this;
}

RGBColor UI_Label::GetBackroundColor() const
{
    return label_background_color;
}

UI_Label& UI_Label::SetColorInverse(bool inverse)
{
    if (label_inverse != inverse)
    {
        label_inverse = inverse;
        Invalidate();
    }
    return *this;
}

void UI_Label::UpdateOffsets()
{
    h_offset = v_offset = 0;
    
    switch (h_align)
    {
    case LabelHorizonAlignment::CENTER:
        h_offset += size.x / 2;
        h_offset -= text_width / 2;
        break;
    case LabelHorizonAlignment::RIGHT:
        h_offset = size.x;
        h_offset -= text_width;
        break;
    case LabelHorizonAlignment::LEFT:
    default:
        break;
    }

    switch (v_align)
    {
    case LabelVerticalAlignment::CENTER:
        v_offset += size.y / 2;
        v_offset -= text_height / 2;
        break;
    case LabelVerticalAlignment::BOTTOM:
        v_offset = size.y;
        v_offset -= text_height;
        break;
    case LabelVerticalAlignment::TOP:
    default:
        break;
    }
}
