/**
 * @file LobbyTask.hpp
 * @date Jun 11, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_TASK_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_TASK_HPP_

#include "LibrariesExport.h"

#include <TaskHandler/PriorityTask.hpp>
#include <Time/Timer.hpp>
#include <Auth/Account.hpp>
#include <Data/StaticText.hpp>

#include <LetoAPI_V1/Web/WebConnection.hpp>

class LIBRARIES_EXPORT LobbyTask : public PriorityTask
{
protected:
	bool Do() override;

public:
	LobbyTask(const StaticText32& name, uint32_t period_ms, uint8_t priority);
};

#endif