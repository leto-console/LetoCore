/**
 * @file LobbyConnection.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_CONNECTION_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_CONNECTION_HPP_

#include <cstdint>

const uint8_t LC_MEMBERS_COUNT = 6;

enum LC_State : uint8_t
{
	LC_STATE_DISCONNECTED 	= 0x00,	///< Подключение отсутствует
	LC_STATE_JOINING		= 0x01,	///< В процессе установки соединения (вход в лобби)
	LC_STATE_CONNECTED		= 0x02,	///< Соединение установлено
};

#pragma pack(push, 2)

/**
 * @brief Callback для обработки получения данных
 *
 * @param[in] id Идентификатор отправителя
 * @param[in] data Данные для получения
 * @param[in] size Размер данных для получения
 */
using LobbyConnection_V1_Callback = void (*)(uint32_t id, const void* data, uint32_t size);

/**
 * @brief Структура с данными о подключении
 */
struct LobbyConnection_V1
{
	/// Создатель лобби (владелец)
    uint32_t owner;
    
	/// Идентификатор приложения
	uint16_t app_id;

    /// Количество подключенных участников (включая создателя)
    uint16_t connected;

    /// Доступное количество участников
    uint16_t max_count;

	/// Участники лобби
	uint32_t members[LC_MEMBERS_COUNT];
	
	/// Callback для обработки получения данных
	LobbyConnection_V1_Callback callback;

	/// Подключение открыто
	uint8_t state;
};

#pragma pack(pop)

#endif /* INC_LETO_API_V1_LOBBY_LOBBY_CONNECTION_HPP_ */
