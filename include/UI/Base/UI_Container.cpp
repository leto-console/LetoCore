#include "UI_Container.hpp"

#include <UI/Base/UI_Themes.hpp>

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

void UI_Container::OnPrev()
{
    if (layer_ptr && layer_ptr->IsSelected()) return;
    if (layer_idx > 0)
    {
        --layer_idx;
        UpdateLayer();
    }
}

void UI_Container::OnNext()
{
    if (layer_ptr && layer_ptr->IsSelected()) return;
    if (layer_idx + 1 < layers.size())
    {
        ++layer_idx;
        UpdateLayer();
    }
}

//                 [--]
//               [--|---]
//            [--|---|-----]
void UI_Container::RefreshAlign()
{
    // Пересобрать сетку (при изменении выравнивания или ещё чего-нибудь)
    int length = 0;
    for (const UI_Object* layer : layers)
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
        switch (GetContainerAlign(true))
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
        layer->Hover(false);
            
    if (IsSelected() && layers.get(layer_idx, layer_ptr)) 
    {
        layer_ptr->Hover(true);
    }
    else
    {
        layer_ptr = nullptr;
    }
}

void UI_Container::ObjectDraw(LetoScreen_V1 *screen)
{
    for (UI_Object* layer : layers)
        layer->Draw(screen);
        
    if (BaseDrawSelection(screen, *this)) return;
    if (BaseDrawHover(screen, *this)) return;
}
