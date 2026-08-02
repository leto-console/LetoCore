/*
 * DrawRectangle.hpp
 *
 *  Created on: Mar 21, 2026
 *      Author: Timur
 */

#ifndef INC_DRAW_FUNCTIONS_DRAW_RECTANGLE_HPP_
#define INC_DRAW_FUNCTIONS_DRAW_RECTANGLE_HPP_

#include "LibrariesExport.h"

#include <Graphics/IScreen.hpp>
#include <Graphics/RGBColor.hpp>

namespace DrawFunctions
{    
    extern LETO_CORE_EXPORT void DrawRectangle(IScreen& screen, Point2_i a, Point2_i b, RGBColor color);

    // Нарисовать обведенный прямоугольник
    extern LETO_CORE_EXPORT void DrawOutlinedRectangle(IScreen& screen, Point2_i a, Point2_i b, RGBColor color, RGBColor outline_color);
}

#endif