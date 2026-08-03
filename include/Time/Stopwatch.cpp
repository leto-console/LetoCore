#include "Stopwatch.hpp"

#include <Time/TimeUtils.hpp>

Stopwatch::Stopwatch(bool auto_start)
{
    if (auto_start) Start();
}

void Stopwatch::Start()
{
    start_mks = TimeUtils::GetCurrentMks();
}

uint32_t Stopwatch::ElapsedMs()
{
    return ElapsedMks() / 1000;
}

uint32_t Stopwatch::ElapsedMks()
{
    if (!start_mks) return 0;
    return TimeUtils::GetCurrentMks() - start_mks;
}
