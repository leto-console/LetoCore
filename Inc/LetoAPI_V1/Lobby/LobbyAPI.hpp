/**
 * @file LobbyAPI_V1.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_API_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_API_HPP_

#include <cstdint>
#include <type_traits>

#include <LetoAPI_V1/Lobby/LobbyConnection.hpp>
#include <LetoAPI_V1/Lobby/LobbyInfo.hpp>

#pragma pack(push, 4)

/// Функции для создания игровых комнат
struct LobbyAPI_V1
{
    /**
	 * @brief Получить список доступных игровых комант рядом
	 * 
	 * @param[out] info Массив для получения информации
	 * @param[in] available Размер массива для получения информации
	 *
	 * @return Количество игровых комнат
	 */
	uint32_t (*GetLobbiesNear) (LobbyInfo_V1* info, uint32_t available);

    /**
     * @brief Создать лобби
     * 
     * @param[out] lobby Открытое лобби
     * @param[in] app_id Идентификатор приложения
     * @param[in] max_count Максимальное количество подключений
     */
    bool (*CreateLobby)(LobbyConnection_V1* lobby, uint16_t app_id, uint8_t max_count, LobbyConnection_V1_Callback callback);

    /**
     * @brief Присоединиться к лобби
     * 
     * @warning Подключение производится асинхронно, о результате можно узнать с помощью `GetActiveLobby`
     * 
     * @param[in] info Информация о лобби
     * @param[in] callback Функция обратного вызова для получения информации из лобби
     */
    void (*JoinLobby)(LobbyInfo_V1 info, LobbyConnection_V1_Callback callback);

    /**
     * @brief Проверить наличие текущего активного лобби
     * 
     * @param[out] lobby Информация об активном лобби
     * 
     * @return `true` - успешное подключение
     */
    bool (*GetActiveLobby)(LobbyConnection_V1* lobby);

    /**
     * @brief Отсоединиться от активного лобби
     */
    void (*QuitLobby)();

    /**
     * @brief Отключить Отключить участника от лобби
     * 
     * @param[in] id Идентификатор участника лобби
     */
    void (*DisconnectMember)(uint32_t id);

    /**
	 * @brief Отправить данные в активное лобби без гарантии доставки
	 *
	 * @param[in] data Данные для отправки
	 * @param[in] size Размер данных для отправки
	 */
	void (*SendData)(const void* data, uint32_t size);

};

#pragma pack(pop)

static_assert(std::is_standard_layout<LobbyAPI_V1>::value, "LobbyAPI_V1 должен быть standard_layout");

#endif
