#include "UI_Alignable.hpp"

NEW_LETO_UI_USING

Point2_i UI_Alignable::AlignRect(Rect_i rect)
{		
    rect.pos.x = 0;
    rect.pos.y = 0;
    
    switch (GetXAlign())
    {
    case UI_XAlign::CENTER:
        rect.pos.x = (GetSize().x - rect.size.x) / 2;
        break;
    case UI_XAlign::RIGHT:
        rect.pos.x = (GetSize().x - rect.size.x);
        break;
    case UI_XAlign::LEFT:
    default:
        break;
    }

    switch (GetYAlign())
    {
    case UI_YAlign::CENTER:
        rect.pos.y = (GetSize().y - rect.size.y) / 2;
        break;
    case UI_YAlign::BOTTOM:
        rect.pos.y = (GetSize().y - rect.size.x);
        break;
    case UI_YAlign::TOP:
    default:
        break;
    }

    return rect.pos;
}

bool UI_Alignable::ObjectOnEvent(const AppEvent &event)
{
    if (IsUIEvent(event, UIE_ON_MOVE) ||
        IsUIEvent(event, UIE_ON_RESIZE))
    {
        RefreshAlign();
    }

    return UI_Object::ObjectOnEvent(event);
}
