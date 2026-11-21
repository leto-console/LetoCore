#include "UI_Object.hpp"

NEW_LETO_UI_USING

void UI_Object::InvalidateRect(const Rect_i& rect)
{
	//leto::graphics::InvalidateRect(rect.pos, rect.size);
}

void UI_Object::Invalidate()
{
    InvalidateRect();
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
        ObjectOnEvent(event);
        OnEventChilds(event);
        OnEventParent(event); //TODO:!!! recursive
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
    {
        // Событие от ребенка идет только вверх по иерархии
        //if (child_event.data & UID_FROM_CHILD) return false;
        child_event.data |= UID_FROM_PARENT;
    }  

    for (UI_Object* child : childs)
    {
        if (child->OnEvent(child_event)) 
            return true;
    }
    return false;
}

bool UI_Object::OnEventParent(const AppEvent &event)
{
    // В родителя отсылаются только UI события, не пришедшие от него же
    if (!IsUIEvent(event) || event.data & UID_FROM_PARENT)
        return false;
    
    AppEvent parent_event = event;
    parent_event.data |= UID_FROM_CHILD;
    return parent && parent->OnEvent(parent_event);
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
	InvalidateRect({pos, size});
}
