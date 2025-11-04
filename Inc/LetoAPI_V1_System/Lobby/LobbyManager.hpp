/**
 * @file LobbyManager.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_MANAGER_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_MANAGER_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <LetoAPI_V1/Lobby/LobbyConnection.hpp>
#include <LetoAPI_V1/Lobby/LobbyConnection.hpp>
#include <LetoAPI_V1/Lobby/LobbyInfo.hpp>

#include <LetoAPI_V1/Web/WebConnection.hpp>
#include <LetoAPI_V1/Web/WebDeviceInfo.hpp>

#include <LetoAPI_V1_System/Web/WebPacket.hpp>
#include <LetoAPI_V1_System/Lobby/LobbyGlobals.hpp>

#include <Data/StaticList.hpp>
#include <Data/RingFIFO_Static.hpp>

#include <Time/Timer.hpp>

/**
 * @brief Класс, обрабатывающий сетевое взаимодействие
 * 
 * @warning Доступен только на уровне OS, при создании в приложении - UB
 */
class LIBRARIES_EXPORT LobbyManager_V1
{
protected:
    // ======================================== Singleton ========================================

	LobbyManager_V1();
	~LobbyManager_V1();
	LobbyManager_V1(const LobbyManager_V1&) = delete;
	void operator=(const LobbyManager_V1&) = delete;

    // ===========================================================================================

    Timer find_timer;
    WebConnection_V1 find_connection;   ///< Подключение к WC_PORT_FND_LOBBY
    WebConnection_V1 main_conection;    ///< Подключение к WC_PORT_CON_LOBBY

    /**
     * @brief Проверить является ли пользователь участником текущего лобби
     * 
     * @param[in] id Идентификатор пользователя
     * @param[out] pos Позиция пользователя в списке
     */
    bool IsMember(uint32_t id, uint32_t* pos = nullptr) const;

    /**
     * @brief Отправить общую информацию об активном лобби
     */
    void SendCommonInfo() const;

    /**
     * @brief Отправить информацию о подключенных участниках
     * 
     * @param[in] part Номер тройки пользователей для отправки (0 или 1)
     * 
     * @warning Функция выполняется сервером
     */
    void SendMembersInfo(uint32_t part) const;

public:
    static inline LobbyManager_V1& Instance()
    {
        static LobbyManager_V1 instance;
        return instance;
    }

	template <typename Tag = void>
	static inline LobbyManager_V1& TagInstance()
    {
        static LobbyManager_V1 instance;
        return instance;
    }

    /** ========================= Функции доступные в LetoAPI_V1 =========================  */

    /**
	 * @brief Получить список доступных игровых комант рядом
	 * 
	 * @param[out] info Массив для получения информации
	 * @param[in] available Размер массива для получения информации
	 *
	 * @return Количество игровых комнат
	 */
    uint32_t GetLobbiesNear(LobbyInfo_V1* info, uint32_t available);

    /**
     * @brief Создать лобби
     * 
     * @param[out] lobby Открытое лобби
     * @param[in] app_id Идентификатор приложения
     * @param[in] max_count Максимальное количество подключений
     */
    bool CreateLobby(LobbyConnection_V1* lobby, uint16_t app_id, uint8_t max_count, LobbyConnection_V1_Callback callback);

    /**
     * @brief Присоединиться к лобби
     * 
     * @param[in] info Информация о лобби
     * @param[in] callback Функция обратного вызова для получения информации из лобби
     */
    void JoinLobby(LobbyInfo_V1 info, LobbyConnection_V1_Callback callback);

    /**
     * @brief Проверить наличие текущего активного лобби
     */
    bool GetActiveLobby(LobbyConnection_V1* lobby);
    
    /**
     * @brief Выйти из текущего лобби
     * 
     * Может быть вызвано как создателем лобби, так и участником
     */
    void QuitLobby();

    /**
     * @brief Отключить участника от лобби
     */
    void DisconnectMember(uint32_t ID);

    /**
	 * @brief Отправить данные в активное лобби без гарантии доставки
	 *
	 * @param[in] data Данные для отправки
	 * @param[in] size Размер данных для отправки
	 */
	void SendData(const void* data, uint32_t size);

    /** ==================================================================================  */

    /**
     * @brief Проверить наличие запроса на подключение к лобби
     */
    bool GetJoiningLobby(LobbyConnection_V1* lobby);

    /**
     * @brief Глобальный сброс состояния
     */
    void GlobalReset();

	/**
	 * @brief Выполнить действия в фоне
	 */
    void Loop();
};


#endif
