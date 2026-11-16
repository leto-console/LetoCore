/**
 * @file UI_Theme.hpp
 * @date Nov 17, 2026
 * @author Rakhimov T.
 */

#pragma once

#include <UI/Base/UI_Object.hpp>

#include <LetoAPI_V1/LetoAPI_V1.h>

#include <LetoFunctions/Draw.hpp>

#include <Graphics/RGBColor.hpp>

struct UI_SelectionTheme
{
public:
    UI_SelectionTheme(LetoColor_V1 bound_color = WhiteColor)
        : bound_color(bound_color)
    { }

    LetoColor_V1 GetBoundColor() const { return bound_color; }

    virtual void DrawSelection(LetoScreen_V1* screen, UI_Object& object) const 
    {
        int32_t x = object.GetPosition().x;
        int32_t y = object.GetPosition().y;
        int32_t width = object.GetSize().x;
        int32_t height = object.GetSize().y;

        if (object.IsSelected())
            leto_api_v1->Graphics->DrawRoundRect(screen, x, y, width, height, 6, 1, bound_color );
    }

protected:
    RGBColor bound_color{ WhiteColor };
};

extern UI_SelectionTheme UI_SelectionTheme_Base;
