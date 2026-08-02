/**
 * @file LobbyGlobals.hpp
 * @date Jun 11, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_GLOBALS_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_GLOBALS_HPP_

#include "LibrariesExport.h"

#include <Data/StaticList.hpp>

#include <LetoAPI_V1/Lobby/LobbyInfo.h>
#include <LetoAPI_V1/Lobby/LobbyConnection.h>
#include <Time/Timer.hpp>

/// @brief Available lobbies nearby
extern LETO_CORE_EXPORT StaticList<LobbyInfo_V1, 8> lobbies_near;

/// @brief Current lobby connection instance
extern LETO_CORE_EXPORT LobbyConnection_V1 global_lobby;

/// @brief Connection timer
extern LETO_CORE_EXPORT Timer join_timer;

#endif
