#include "UI_Object.hpp"

void UI_Object::Invalidate()
{
    //leto::graphics::InvalidateRect(pos, size);
    for (UI_Object* child : childs)
        child->Invalidate();
    SendUIEvent(UIE_ON_INVALIDATE);
}

void UI_Object::Draw(LetoScreen_V1 *screen)
{
    if (!IsEnabled()) return;
    /// Самый передний объект отрисовывается 
    /// самым последним
    for (UI_Object* child : childs)
        child->Draw(screen);
    ObjectDraw(screen);
}

void UI_Object::Loop()
{
    if (!IsEnabled()) return;
    ObjectLoop();
    for (UI_Object* child : childs)
        child->Loop();
}

bool UI_Object::OnEvent(const AppEvent &event)
{
    // TODO: Подумать над архитекутурой распространения событий
    if (!IsEnabled() && !IsUIEvent(event, UIE_ON_ENABLE)) return false;
    if (ObjectOnEvent(event)) return true;

    return OnEventChilds(event);
}
