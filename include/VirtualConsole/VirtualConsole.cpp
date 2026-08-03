#include "VirtualConsole.hpp"

#include <Input/SystemInputID.hpp>

void VirtualConsole::ResetViewed()
{
    for (VC_Message& msg : msgs)
    {
        msg.viewed = msg.set_viewed;
    }
    bottom_offset = 0;
}

bool VirtualConsole::ProcessInput(const AppEvent &event)
{
    if (IsSystemNextEvent(event) && (bottom_offset != 0)) bottom_offset--;
    else if (IsSystemPrevEvent(event) && (bottom_offset + VC_LINE_SHOW < msgs.Count() - 1)) bottom_offset++;
    
    return true;
}

void VirtualConsole::Draw(IScreen &screen, Point2_i offset)
{
    static StaticText8 delim = "#";
    size_t txt_area_width = 160 - 10 - 6;

    /// TODO: Warning: HARD_CODE value of ST7735
    Point2_i point_a{5, 5}, point_b = {160 - 5, 8 * VC_LINE_SHOW + 5 + 3};
    Point2_i point_txt = point_a + Point2_i{3, 8 * (VC_LINE_SHOW - 1) + 3};

    DrawFunctions::DrawOutlinedRectangle(screen, point_a, point_b, BlackColor, GrayColor);

    size_t line = 0, skip = bottom_offset;

    // Перебор сообщений от самых новых к самым старым
    for (VC_Message& msg : msgs.reversed())
    {
        if (skip)
        {
            skip--;
            continue;
        }

        size_t length;
        if (length = msg.text.TextLength())
        {
            DrawFunctions::DrawText(screen, point_txt, delim, msg.viewed ? DarkGrayColor : WhiteColor, BlackColor, false, VC_FONT);
            DrawFunctions::DrawText(screen, point_txt + Point2_i{12, 0}, msg.text, msg.color, BlackColor, false, VC_FONT);

            line++;
        }

        point_txt.y -= VC_FONT->GetHeight();
        msg.set_viewed = true;

        if (line > VC_LINE_SHOW) break;
    }
}

