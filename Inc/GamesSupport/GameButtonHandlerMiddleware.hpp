/*
 * GameButtonHandlerMiddleware.hpp
 *
 *  Created on: Jan 13, 2025
 *      Author: Timur
 */

#ifndef INC_GAMES_SUPPORT_GAME_BUTTON_HANDLER_MIDDLEWARE_HPP_
#define INC_GAMES_SUPPORT_GAME_BUTTON_HANDLER_MIDDLEWARE_HPP_

#include "LibrariesExport.h"

#include <ABI/AppEvent.hpp>
#include <UI/ButtonHoldHandler.hpp>
#include <UI/ButtonMultiPressHandler.hpp>

#include <Input/IsEventFunc.hpp>

namespace GameButtonHandlerMiddleware
{
	extern LETO_CORE_EXPORT void HandleEvent(const AppEvent& event, IsEventFunc is_event, ButtonHoldHandler& handler);
	extern LETO_CORE_EXPORT void HandleEvent(const AppEvent& event, IsEventFunc is_event, ButtonMultiPressHandler& handler, int button);
};

#endif
