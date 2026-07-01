/**
 * @file StackGuard.hpp
 * @date Jun 30, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_STACK_GUARD_HPP__
#define INC_SYSTEM_STACK_GUARD_HPP__

#include "LibrariesExport.h"

#include <cstdint>
#include <Graphics/IDisplay.hpp>
#include <Graphics/IDrawer.hpp>
#include <Data/StaticText.hpp>
#include <DrawFunctions/DrawText.hpp>

/**
 * @brief Защитник стековой памяти 
 * 
 * Размещать его необходимо после _end и самым последним heap-объектов (при наличии)
 *
 * @verbatim
 * ############################################################################
 * #  .data  #  .bss  #          #              #          MSP stack          #
 * #         #        #          #              # Reserved by _Min_Stack_Size #
 * ############################################################################
 * ^-- RAM start      ^-- _end   ^-- Guard                 _estack, RAM end --^
 * @endverbatim
 */

class IndigoScreenDrawer : public IDrawer
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


class LIBRARIES_EXPORT StackGuard
{
public:
    /// 512 byte guard
    enum : uint32_t { PATTERN = 0xC1A0BABE };  
    enum : uint8_t { PATTERN_SIZE = 64, BIG_PROBLEM_SIZE = 8 };

    StackGuard();

    uint16_t Check();

private:
    volatile uint32_t pattern[PATTERN_SIZE];
};

extern LIBRARIES_EXPORT StackGuard* SystemStackGuard0;   ///< Разрушение его лишает возможности отрисовывать что-либо на дисплее
extern LIBRARIES_EXPORT StackGuard* SystemStackGuard1;   ///< Разрушение его лишает возможности использовать переменные из heap/bss до дисплея

extern LIBRARIES_EXPORT void SystemStackGuardLoop();

//#ifdef USE_HAL_DRIVER
//__attribute__((section("..."), used, aligned(4)))
//#endif
//extern LIBRARIES_EXPORT StackGuard MainStackGuard;

#endif
