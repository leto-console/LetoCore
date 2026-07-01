#include "StackGuard.hpp"

#include <System/SystemDisplay.hpp>
#include <LetoAPI_V1/LetoAPI_V1.hpp>

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

static void ShowIndigoScreen(uint8_t size0, uint8_t size1)
{
    if (!SystemDisplay) return;

    char txt[20];
    IndigoScreenDrawer indigo;
    indigo.SetMessage("STACK OVERFLOW");

    leto_api_v1->Text->FormatText(txt, sizeof(txt), "%03d:%03d [%03d]", size0, size1, StackGuard::PATTERN_SIZE);
    indigo.SetData(txt);

    SystemDisplay->Render(&indigo);
    SystemDisplay->Loop();
}

static void MemoryFault(uint8_t size0, uint8_t size1, bool big_problem = false)
{
    if (big_problem) goto loop;

    // Вызов какого-нибудь прерывания
    // Запрет на другие прерывания
    // При возможности обновить состояние дисплея (написать о MemoryFault)
    // Скорее всего возможности такой не будет, поскольку стек уже разрушен (но нужно проанализировать этот момент)
    //
    // Проанализировано: несмотря на то, что стек разрушен, если он не разрушил ключевую рабочую логику (отображения)
    // можно попробовать отрисовать сообщение об ошибке - синий экран - и свалиться в ошибку
    // Для этого нужен предварительный анализ, что разрушено

    ShowIndigoScreen(size0, size1);
    
    loop:

#ifdef USE_HAL_DRIVER
    while (1) { }
#else
    throw "StackGuard::MemoryFault";
#endif
}

StackGuard* SystemStackGuard0 = nullptr;
StackGuard* SystemStackGuard1 = nullptr;

void SystemStackGuardLoop()
{
    if (!SystemStackGuard0 || !SystemStackGuard1) return;

    uint16_t check0 = SystemStackGuard0->Check();
    uint16_t check1 = SystemStackGuard1->Check();

    if (!check0 && !check1) return;

    // Большая проблема в области отрисовки дисплея
    if (check0 && (check0 & 0xFF == 2))
        MemoryFault(0, 0, true);

    MemoryFault(check0 >> 8, check1 >> 8);
}
