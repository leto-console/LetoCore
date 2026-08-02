/*
 * InputCatcher.hpp
 *
 *  Created on: Mar 15, 2026
 *      Author: Timur
 */

#ifndef INC_INPUT_IS_EVENT_FUNC_HPP_
#define INC_INPUT_IS_EVENT_FUNC_HPP_

#include <LetoABI/AppEvent.h>

using IsEventFunc = bool(*)(const AppEvent& e, bool p);

#endif
