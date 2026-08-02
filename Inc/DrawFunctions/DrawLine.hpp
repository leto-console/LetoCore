/*
 * DrawLine.hpp
 *
 *  Created on: Mar 21, 2026
 *      Author: Timur
 */

#ifndef INC_DRAW_FUNCTIONS_DRAW_LINE_HPP_
#define INC_DRAW_FUNCTIONS_DRAW_LINE_HPP_

#include "LibrariesExport.h"

#include <Graphics/IScreen.hpp>
#include <Graphics/RGBColor.hpp>

namespace DrawFunctions
{
    extern LETO_CORE_EXPORT void DrawLine(IScreen& screen, Point2_i a, Point2_i b, RGBColor color);
}

#endif