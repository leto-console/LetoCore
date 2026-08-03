#include "LobbyGlobals.hpp"

StaticList<LobbyInfo_V1, 8> lobbies_near{};

LobbyConnection_V1 global_lobby{};

bool global_lobby_is_connecting{ false };

Timer join_timer;
