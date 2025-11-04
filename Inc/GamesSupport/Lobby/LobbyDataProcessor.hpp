/**
 * @file LobbyDataProcessor.hpp
 * @date Jun 14, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_DATA_PROCESSOR_HPP_
#define INC_GAMES_SUPPORT_LOBBY_DATA_PROCESSOR_HPP_

#include <Data/StaticList.hpp>

#include <GamesSupport/Lobby/LobbyData.hpp>

class LIBRARIES_EXPORT LobbyDataProcessor
{
public:
    LobbyDataProcessor() = default;

    void RegLobbyData(LobbyData* data);

    void UnregLobbyData(LobbyData* data);

    /// Получить данные (вызывается в callback)
    void ReceiveData(uint32_t id, const void* data, uint32_t size);

    /// Функцию необходимо вызывать в цикле
    void Loop();

protected:
    StaticList<LobbyData*, 16> pool;
};

#endif
