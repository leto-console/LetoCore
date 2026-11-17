#include "UI_Object.hpp"

NEW_LETO_UI_USING

void UI_Object::Invalidate()
{
    InvalidateRect();
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
    // События UI распространяются на все элементы без возможности захватить их
    if (IsUIEvent(event))
    {
        if (IsUIEvent(event, UIE_ON_INVALIDATE, UID_NOT_SELF)) 
            InvalidateRect();
        ObjectOnEvent(event);
        OnEventChilds(event);
        return false;
    }
    // Прочие события только для включенного объекта и имеют возможность захвата
    else if (IsEnabled())
    {
        return ObjectOnEvent(event) || OnEventChilds(event);
    }

    return false;
}

bool UI_Object::OnEventChilds(const AppEvent &event)
{
    AppEvent child_event = event;
    if (IsUIEvent(child_event))        
        child_event.data |= UID_NOT_SELF;

    for (UI_Object* child : childs)
    {
        if (child->OnEvent(child_event)) 
            return true;
    }
    return false;
}

bool UI_Object::SetFlag(uint8_t flag, bool state)
{
    if (flag >= 8u * sizeof(flags)) return false;

    bool prev_state = CheckFlag(flag);
    if (prev_state == state) return false;
    
    if (state)
        flags |= (1u << flag);
    else
        flags &= ~(1u << flag);

    return true;
}

bool UI_Object::CheckFlag(uint8_t flag) const
{
    if (flag >= 8u * sizeof(flags)) return false;
    
    return (flags >> flag) & 1u;
}

void UI_Object::InvalidateRect()
{
    //leto::graphics::InvalidateRect(pos, size);
}
