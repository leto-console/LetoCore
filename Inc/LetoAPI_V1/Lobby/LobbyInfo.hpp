/**
 * @file LobbyInfo.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_INFO_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_INFO_HPP_

#include <cstdint>

#pragma pack(push, 2)

/**
 * @brief Информация об игровой команте
 */
struct LobbyInfo_V1
{
    /// Создатель лобби (владелец)
    uint32_t owner;
    
	/// Идентификатор приложения
	uint16_t app_id;

    /// Количество подключенных участников (включая создателя)
    uint16_t connected;

    /// Доступное количество участников
    uint16_t max_count;

    /// Последняя передача информации о комнате
    uint32_t last_info;
};

#pragma pack(pop)

#endif /* INC_LETO_API_V1_LOBBY_LOBBY_INFO_HPP_ */
