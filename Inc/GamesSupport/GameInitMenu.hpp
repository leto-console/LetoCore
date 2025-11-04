/*
 * GameInitMenu.hpp
 *
 *  Created on: Mar 15, 2026
 *      Author: Timur
 */

#ifndef INC_GAME_SUPPORT_GAME_INPUT_MENU_HPP_
#define INC_GAME_SUPPORT_GAME_INPUT_MENU_HPP_

#include <UI/Menu/Menu.hpp>
#include <UI/Menu/DialogMenu.hpp>
#include <GamesSupport/GameInputID.hpp>

// Базовая инициализация меню игровым вводом
template <uint8_t MaxSize>
inline void BaseGameInitMenu(Menu<MaxSize>& menu)
{
    menu.RegUpEvent(GameUpEvent());
    menu.RegUpEvent(GameTurnLeftEvent());
    menu.RegDownEvent(GameDownEvent());
    menu.RegDownEvent(GameTurnRightEvent());
}

// Базовая инициализация диалогового окна игровым вводом
template <typename Param, uint8_t MaxSize>
inline void BaseGameInitDialog(DialogParamMenu<Param, MaxSize>& dialog)
{
    dialog.RegUpEvent(GameUpEvent());
    dialog.RegUpEvent(GameTurnLeftEvent());
    dialog.RegDownEvent(GameDownEvent());
    dialog.RegDownEvent(GameTurnRightEvent());
    dialog.RegEnterEvent(GameEnterEvent());
    dialog.RegEnterEvent(GameRightEvent());
}

#endif
