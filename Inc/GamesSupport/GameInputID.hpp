/*
 * GameInputID.hpp
 *
 *  Created on: Jan 24, 2026
 *      Author: Timur
 */

#ifndef INC_GAME_SUPPORT_GAME_INPUT_ID_HPP_
#define INC_GAME_SUPPORT_GAME_INPUT_ID_HPP_

#include <stdint.h>
#include <Input/SystemInputID.hpp>

#include <Input/ButtonEvent.hpp>
#include <Input/EncoderEvent.hpp>
#include <Input/DiscreteSignalEventTypes.hpp>

enum GAME_INPUT_IDs : uint8_t
{
	GAME_BTN_UP = SYSTEM_BTN_UP,
	GAME_BTN_DOWN = SYSTEM_BTN_DOWN,
	GAME_BTN_LEFT = SYSTEM_BTN_LEFT,
	GAME_BTN_RIGHT = SYSTEM_BTN_RIGHT,
	GAME_BTN_ENTER = SYSTEM_BTN_ENTER,

	GAME_ENC_MAIN = SYSTEM_ENC_MAIN,
};

static uint8_t GameButtonIds[] = {
    GAME_BTN_UP, GAME_BTN_DOWN, GAME_BTN_LEFT, GAME_BTN_RIGHT, GAME_BTN_ENTER
};

constexpr inline AppEvent GameLeftEvent(bool pressed = true)
{
    return { AE_Button, GAME_BTN_LEFT, pressed ? ButtonEvent::PRESSED : ButtonEvent::RELEASED };
}

constexpr inline AppEvent GameRightEvent(bool pressed = true)
{
    return { AE_Button, GAME_BTN_RIGHT, (uint8_t) (pressed ? ButtonEvent::PRESSED : ButtonEvent::RELEASED) };
}

constexpr inline AppEvent GameUpEvent(bool pressed = true)
{
    return { AE_Button, GAME_BTN_UP, (uint8_t) (pressed ? ButtonEvent::PRESSED : ButtonEvent::RELEASED) };
}

constexpr inline AppEvent GameDownEvent(bool pressed = true)
{
    return { AE_Button, GAME_BTN_DOWN, (uint8_t) (pressed ? ButtonEvent::PRESSED : ButtonEvent::RELEASED) };
}

constexpr inline AppEvent GameEnterEvent(bool pressed = true)
{
    return { AE_Button, GAME_BTN_ENTER, (uint8_t) (pressed ? ButtonEvent::PRESSED : ButtonEvent::RELEASED) };
}

constexpr inline AppEvent GameTurnLeftEvent()
{
    return { AE_Encoder, GAME_ENC_MAIN, (uint8_t) EncoderEvent::TURN_LEFT };
}

constexpr inline AppEvent GameTurnRightEvent()
{
    return { AE_Encoder, GAME_ENC_MAIN, (uint8_t)EncoderEvent::TURN_RIGHT };
}

#endif
