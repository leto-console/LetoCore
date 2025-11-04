/**
 * @file LobbyGlobals.hpp
 * @date Jun 11, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_GLOBALS_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_GLOBALS_HPP_

#include "LibrariesExport.h"

#include <Data/StaticList.hpp>

#include <LetoAPI_V1/Lobby/LobbyInfo.hpp>
#include <LetoAPI_V1/Lobby/LobbyConnection.hpp>
#include <Time/Timer.hpp>

/// @brief Доступные лобби рядом
extern LIBRARIES_EXPORT StaticList<LobbyInfo_V1, 8> lobbies_near;

/// @brief Текущее лобби
extern LIBRARIES_EXPORT LobbyConnection_V1 global_lobby;

/// @brief Таймер подключения
extern LIBRARIES_EXPORT Timer join_timer;

#endif
