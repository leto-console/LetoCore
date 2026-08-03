#include "StackGuard.hpp"

#include "IndigoScreen.hpp"
#include "SystemDisplay.hpp"

#include <cstdio>

StackGuard::StackGuard()
{
    for (volatile uint32_t& e : pattern) e = PATTERN;
}

uint16_t StackGuard::Check()
{
    uint16_t check_result{};

    for (uint8_t i = 0; i < BIG_PROBLEM_SIZE; ++i)
    {
        if (pattern[i] == PATTERN) continue;

        // Стек сильно испорчен (затерты первые 8 блоков - 32 байта)
        check_result |= 2;
        check_result |= (PATTERN_SIZE - i) << 8;

        goto result;
    }

    for (uint8_t i = BIG_PROBLEM_SIZE; i < PATTERN_SIZE; ++i)
    {
        if (pattern[i] == PATTERN) continue;

        // Стек испорчен (но есть вероятно, что не тронуты переменные до guard'а)
        check_result |= 1;
        check_result |= (PATTERN_SIZE - i) << 8;

        goto result;
    }

    result:
    return check_result;
}


static void MemoryFault(uint8_t size0, uint8_t size1)
{    
    static char data[20];
    snprintf(data, sizeof(data), "%03d:%03d [%03d]", size0, size1, StackGuard::PATTERN_SIZE);

    CrashIndigoScreen("STACK OVERFLOW", data);
}

StackGuard* SystemStackGuard = nullptr;

void SystemStackGuardLoop()
{
    if (!DisplayStackGuard || !SystemStackGuard) return;

    uint16_t check0 = DisplayStackGuard->Check();
    uint16_t check1 = SystemStackGuard->Check();

    if (!check0 && !check1) return;

    // Большая проблема в области отрисовки дисплея
    if (check0 && (check0 & 0xFF == 2))
        MemoryFault(0, 0);

    MemoryFault(check0 >> 8, check1 >> 8);
}
