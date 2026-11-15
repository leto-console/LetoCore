/**
 * @file TestUI.hpp
 * @date Nov 15, 2026
 * @author Rakhimov T.
 */

#pragma once

#include "LetoCoreExport.h"

#include <Graphics/IScreen.hpp>
#include <LetoABI/AppEvent.h>

extern LETO_CORE_EXPORT void TestUI_Init();

extern LETO_CORE_EXPORT void TestUI_DeInit();

extern LETO_CORE_EXPORT void TestUI_Draw(IScreen& screen);

extern LETO_CORE_EXPORT void TestUI_OnEvent(const AppEvent& event);

extern LETO_CORE_EXPORT void TestUI_Loop();
