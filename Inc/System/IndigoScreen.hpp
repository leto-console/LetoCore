/**
 * @file IndigoScreen.hpp
 * @date Jul 11, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_INDIGO_SCREEN_HPP__
#define INC_SYSTEM_INDIGO_SCREEN_HPP__

#include "LibrariesExport.h"

#include <Graphics/IDrawer.hpp>
#include <DrawFunctions/DrawText.hpp>

class LETO_CORE_EXPORT IndigoScreenDrawer : public IDrawer
{
public:
    IndigoScreenDrawer()
    {
        restart[0] = "ПЕРЕЗАПУСТИТЕ";
        restart[1] = "КОНСОЛЬ";
    }

    void SetMessage(const StaticText32& set_message) { message = set_message; } 
    void SetData(const StaticText32& set_data) { data = set_data; }

    void Draw(IScreen& screen)
    {
        screen.FillScreen(IndigoColor);
        DrawFunctions::DrawText(screen, { 30, 30 }, message, WhiteColor, RedColor);
        DrawFunctions::DrawText(screen, { 30, 50 }, restart[0], WhiteColor, BlackColor);
        DrawFunctions::DrawText(screen, { 30, 60 }, restart[1], WhiteColor, BlackColor);
        DrawFunctions::DrawText(screen, { 30, 80 }, data, WhiteColor, BlackColor, true);
    }

    bool Loop() { return true; }

protected:
    StaticText32 restart[2];
    StaticText32 message;
    StaticText32 data;
};

extern LETO_CORE_EXPORT void CrashIndigoScreen(const StaticText32& message, const StaticText32& data = {});

#endif
