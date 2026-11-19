#include "UI_Container.hpp"

NEW_LETO_UI_USING

bool UI_Container::ObjectOnEvent(const AppEvent &event)
{            
    UI_Alignable::ObjectOnEvent(event);

    if (IsUIEvent(event, UIE_ON_SELECT))
    {
        UpdateLayer();
    }

    return layer_ptr && layer_ptr->OnEvent(event);
}

//                 [--]
//               [--|---]
//            [--|---|-----]
void UI_Container::RefreshAlign()
{
    // Пересобрать сетку (при изменении выравнивания или ещё чего-нибудь)
    int length = 0;
    for (UI_Object* layer : layers)
    {
        length += GetAxis(layer->GetSize());
        length += margin;
    }

    Point2_i align_offset{};
    switch (GetContainerAlign())
    {
    case UI_ContainerAlign::Center:
        GetAxis(align_offset) = (GetAxis(GetSize()) - length) / 2;
        break;
    case UI_ContainerAlign::End:
        GetAxis(align_offset) = GetAxis(GetSize()) - length;
        break;
    };

    Point2_i p = GetPosition() + align_offset;
    for (UI_Object* layer : layers)
    {
        switch (GetContainerAlign())
        {
        case UI_ContainerAlign::Center:
            GetAxis(p, true) = (GetAxis(GetSize(), true) - GetAxis(layer->GetSize(), true)) / 2;
            break;
        case UI_ContainerAlign::End:
            GetAxis(p, true) = (GetAxis(GetSize(), true) - GetAxis(layer->GetSize(), true));
            break;
        };

        layer->SetPosition(p);
        
        GetAxis(p) += GetAxis(layer->GetSize());
        GetAxis(p) += margin;
    }

    Invalidate();
}

void UI_Container::UpdateLayer()
{		
    for (UI_Object* layer : layers)
        layer->Select(false);
            
    if (IsSelected() && layers.get(layer_idx, layer_ptr)) 
    {
        layer_ptr->Select(true);
    }
    else
    {
        layer_ptr = nullptr;
    }
}
