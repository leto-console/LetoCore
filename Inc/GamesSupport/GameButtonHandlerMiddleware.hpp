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

namespace GameButtonHandlerMiddleware
{
	extern LIBRARIES_EXPORT void HandleEvent(const AppEvent& event, const AppEvent& source, ButtonHoldHandler& handler);
	extern LIBRARIES_EXPORT void HandleEvent(const AppEvent& event, const AppEvent& source, ButtonMultiPressHandler& handler, int button);
};

#endif
