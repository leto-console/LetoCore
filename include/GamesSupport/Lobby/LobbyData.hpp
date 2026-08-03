/**
 * @file GameLobbyData.hpp
 * @date Jun 14, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_LOBBY_DATA_HPP_
#define INC_GAMES_SUPPORT_LOBBY_LOBBY_DATA_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <cstring>

#include <Time/Timer.hpp>

class LETO_CORE_EXPORT LobbyData
{
public:
    LobbyData(uint8_t data_id, uint32_t period_ms);

    /// Подготовить данные для отправки
    bool PrepareSend(void* data, uint32_t size);

    /// Обработать полученные данные
    void ProcessReceive(uint32_t id, const void* data, uint32_t size);

protected:
    /**
     * @brief Отправить данные
     */
    virtual bool OnSend(void* data, uint32_t size) = 0;

    /**
     * @brief Получить данные
     */
    virtual void OnReceive(uint32_t id, const void* data, uint32_t size) = 0;

private:
    uint8_t data_id;        ///< Идентификатор данных
    uint32_t period_ms;     ///< Период передачи и приема данных

    Timer send_timer;

    bool CheckTimer() const { return send_timer.Expired(false); }
    void RestartTimer() { send_timer.Start(); }
};

#endif /* INC_GAMES_SUPPORT_LOBBY_LOBBY_DATA_HPP_ */
