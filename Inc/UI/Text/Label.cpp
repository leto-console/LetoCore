#include "Label.hpp"

#include <DrawFunctions/DrawText.hpp>

Label::Label(Point2_i pos, Point2_i size)
{
    SetPosition(pos);
    SetWidth(size.x);
    SetHeight(size.y);

    text_height = DrawFunctions::GetDefaultFont()->GetHeight();
}

void Label::Draw(IScreen &screen, Point2_i offset)
{
    using namespace DrawFunctions;

    Point2_i _draw_pos = position + offset + Point2_i(h_offset, v_offset);
    
    DrawText(screen, _draw_pos, label_text, label_text_color, label_background_color, label_inverse, label_font);
}

bool Label::ProcessInput(const AppEvent &event)
{
    (void) event;
    return false;
}

void Label::SetText(const StaticText32 &text)
{
    label_text = text;
    text_width = DrawFunctions::TextWidth(label_text, label_font);
    Invalidate();
}

StaticTextView Label::GetText() const
{
    return label_text;
}

Label& Label::SetHorizonAlignment(LabelHorizonAlignment alignment)
{
    if (alignment == h_align) return *this;

    h_align = alignment;

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

    Invalidate();
    return *this;
}

Label& Label::SetVerticalAlignment(LabelVerticalAlignment alignment)
{
    if (alignment == v_align) return *this;

    v_align = alignment;

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

    Invalidate();
    return *this;
}

Label& Label::SetFont(IFont *font)
{
    if (font)
    {
        label_font = font;
        text_height = label_font->GetHeight();
        Invalidate();
    }
    return *this;
}

Label& Label::SetTextColor(RGBColor color)
{
    if (label_text_color != color)
    {
        label_text_color = color;
        Invalidate();
    }
    return *this;
}

Label& Label::SetBackroundColor(RGBColor color)
{
    if (label_background_color != color)
    {
        label_background_color = color;
        Invalidate();
    }
    return *this;
}

Label& Label::SetColorInverse(bool inverse)
{
    if (label_inverse != inverse)
    {
        label_inverse = inverse;
        Invalidate();
    }
    return *this;
}
