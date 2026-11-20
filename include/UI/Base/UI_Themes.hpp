/**
 * @file UI_Theme.hpp
 * @date Nov 17, 2026
 * @author Rakhimov T.
 */

#pragma once

#include "LetoCoreExport.h"

#include <UI/Base/UI_Object.hpp>

#include <LetoAPI_V1/LetoAPI_V1.h>

#include <LetoFunctions/Draw.hpp>

#include <Graphics/RGBColor.hpp>

NEW_LETO_UI_BEGIN

extern LETO_CORE_EXPORT bool BaseDrawSelection(LetoScreen_V1* screen, UI_Object& object);
extern LETO_CORE_EXPORT bool BaseDrawHover(LetoScreen_V1* screen, UI_Object& object);

NEW_LETO_UI_END