/**
 * @file VirtualConsole.hpp
 * @date May 15, 2026
 * @author Rakhimov T.
 */

#ifndef INC_VIRTUAL_CONSOLE_VIRTUAL_CONSOLE_HPP__
#define INC_VIRTUAL_CONSOLE_VIRTUAL_CONSOLE_HPP__

#include "LibrariesExport.h"

#include <Data/StaticTextView.hpp>
#include <Data/StaticList.hpp>
#include <Graphics/IScreen.hpp>
#include <SceneManager/ISceneObject.hpp>
#include <Graphics/DefaultFont.hpp>
#include <DrawFunctions/DrawText.hpp>
#include <DrawFunctions/DrawRectangle.hpp>

#include <stdarg.h>

/// Шрифт сообщений
#define VC_FONT (&Default_Font_7x7_small)

/// Количество сохраняемых сообщений
#define VC_MSGS_COUNT 32

/// Количество отображаемых строк
#define VC_LINE_SHOW 8

struct VC_Message
{
    StaticText32 text;
    RGBColor color = WhiteColor;
    bool viewed = false;
    mutable bool set_viewed = false;
};

class LIBRARIES_EXPORT VirtualConsole : public ISceneObject
{
protected:
    StaticList<VC_Message, VC_MSGS_COUNT> msgs;
    size_t bottom_offset{};

public:
    static VirtualConsole& Instance()
    {
        static VirtualConsole instance;
        return instance;
    }

    void Print(StaticText32 text, RGBColor color = WhiteColor)
    {
        if (msgs.Full())
        {
            msgs.Erase(&msgs.Front());
        }
        msgs.Push({text, color});
    }

    // Внутренняя версия, работающая с va_list
    void VPrintf(const char* fmt, RGBColor color, va_list args) 
    {
        char buff[256];
        vsnprintf(buff, sizeof(buff), fmt, args);
        Print(buff, color); // ваша функция вывода
    }

    void Printf(const char* fmt, RGBColor color, ...)
    {
        va_list args;
        va_start(args, color);
        VPrintf(fmt, color, args);
        va_end(args);
    }

    void ResetViewed();

    bool ProcessInput(const AppEvent& event) override;

    void Draw(IScreen& screen, Point2_i offset = {}) override;
};

// Inline-функция для удобного вывода текста
inline void VC_Print(const char* text, RGBColor clr)
{
    VirtualConsole::Instance().Print(text, clr);
}

// Inline-функция для удобного вывода текста
inline void VC_Printf(const char* fmt, RGBColor clr, ...)
{
    va_list args;
    va_start(args, clr);
	VirtualConsole::Instance().VPrintf(fmt, clr, args);
    va_end(args);
}

#endif
