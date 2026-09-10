#include "DebugMode.hpp"

static IDataCell<bool>* DebugCell{};

void InitDebugModeCell(IDataCell<bool> *cell)
{
    DebugCell = cell;
}

void SetDebugMode(bool debug)
{
    if (DebugCell) DebugCell->Set(debug);
}

bool GetDebugMode()
{
    return DebugCell && DebugCell->GetOrDefault();
}
