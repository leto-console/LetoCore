#include "DebugMode.hpp"

static IDataCell<bool>* DebugCell{};

void InitDebugModeCell(IDataCell<bool> *cell)
{
    DebugCell = cell;
}

bool GetDebugMode()
{
    return DebugCell && DebugCell->GetOrDefault();
}
