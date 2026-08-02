/*
 * SystemInputID.hpp
 *
 *  Created on: Jan 24, 2026
 *      Author: Timur
 */

#ifndef INC_INPUT_SYSTEM_INPUT_ID_HPP_
#define INC_INPUT_SYSTEM_INPUT_ID_HPP_

#include "LibrariesExport.h"

#include <stdint.h>

#include <LetoABI/AppEvent.h>

enum SYSTEM_INPUT_IDs : uint8_t
{
	__SYSTEN_BTN_IDs = 0,
	SYSTEM_BTN_UP,			///< Клавиша вверх
	SYSTEM_BTN_DOWN,		///< Клавиша вниз
	SYSTEM_BTN_LEFT,		///< Клавиша влева
	SYSTEM_BTN_RIGHT,		///< Клавиша вправо
	SYSTEM_BTN_ENTER,		///< Клавиша Enter
	SYSTEM_BTN_MENU,		///< Клавиша Меню
	SYSTEM_BTN_ALT,			///< Клавиша Alt (B)

	__SYSTEN_ENC_IDs = 128,
	SYSTEM_ENC_MAIN,		///< Главный энкодер
};

//....

extern LETO_CORE_EXPORT bool IsSystemLeftEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemRightEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemTurnLeftEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemTurnRightEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemUpEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemDownEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemPrevEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemNextEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemReturnEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemEnterEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemMenuEvent(const AppEvent& event, bool pressed = true);
extern LETO_CORE_EXPORT bool IsSystemAltEvent(const AppEvent& event, bool pressed = true);

#endif
