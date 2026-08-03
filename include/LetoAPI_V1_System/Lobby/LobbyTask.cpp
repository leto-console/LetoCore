#include "LobbyTask.hpp"

#include "LobbyManager.hpp"

bool LobbyTask::Do()
{
    LobbyManager_V1::Instance().Loop();
    return true;
}

LobbyTask::LobbyTask(const StaticText32 &name, uint32_t period_ms, uint8_t priority)
    : PriorityTask{name, period_ms, priority}
{
    LobbyManager_V1::Instance().GlobalReset();
}

