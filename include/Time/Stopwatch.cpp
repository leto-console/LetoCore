#include "Stopwatch.hpp"

#include <LetoFunctions/Globals.hpp>

Stopwatch::Stopwatch(bool auto_start)
{
    if (auto_start) Start();
}

void Stopwatch::Start()
{
    start_mks = leto::globals::GetCurrentMcs();
}

uint32_t Stopwatch::ElapsedMs()
{
    return ElapsedMks() / 1000;
}

uint32_t Stopwatch::ElapsedMks()
{
    if (!start_mks) return 0;
    return leto::globals::GetCurrentMcs() - start_mks;
}
