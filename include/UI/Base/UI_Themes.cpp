#include "UI_Themes.hpp"

#include <Graphics/RGBColor.hpp>

NEW_LETO_UI_USING

bool LetoNewUI::BaseDrawSelection(LetoScreen_V1 *screen, UI_Object &object)
{
    if (object.IsSelected())
    {
        int32_t x = object.GetPosition().x;
        int32_t y = object.GetPosition().y;
        int32_t width = object.GetSize().x;
        int32_t height = object.GetSize().y;

        leto_api_v1->Graphics->DrawRoundRect(screen, x, y, width, height, 6, 1, GrayColor );
        return true;
    }

    return false;
}

bool LetoNewUI::BaseDrawHover(LetoScreen_V1 *screen, UI_Object &object)
{
    if (object.IsHovered())
    {
        int32_t x = object.GetPosition().x;
        int32_t y = object.GetPosition().y;
        int32_t width = object.GetSize().x;
        int32_t height = object.GetSize().y;

        leto_api_v1->Graphics->DrawRoundRect(screen, x, y, width, height, 6, 1, WhiteColor );
        return true;
    }

    return false;
}
