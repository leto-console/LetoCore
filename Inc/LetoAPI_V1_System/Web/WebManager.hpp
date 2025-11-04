/**
 * @file WebManager.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_MANAGER_HPP_
#define INC_LETO_API_V1_WEB_WEB_MANAGER_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <LetoAPI_V1/Web/WebConnection.hpp>
#include <LetoAPI_V1/Web/WebDeviceInfo.hpp>

#include <LetoAPI_V1_System/Web/WebPacket.hpp>

#include <Data/StaticList.hpp>
#include <Data/RingFIFO_Static.hpp>

#include <Time/Timer.hpp>

const uint32_t POOL_SIZE = 8;

struct WebPacket_BufferItem
{
	WebPacket_V1 packet;			///< Пакет
	uint8_t channel;				///< Канал
};

struct WebPacket_SyncItem
{
	WebPacket_BufferItem buffer_item;	///< Пакет и канал
	uint32_t last_send_ms;				///< Время последней отправки
};

/**
 * @brief Вспомогательный объект для контроля открытых соединений
 */
struct WebConnection_PoolItem
{
	WebConnection_V1 connection;    ///< Данные по соединению
    uint32_t last_active_ms;        ///< Время последней активности соединения
};

/**
 * @brief Класс, обрабатывающий сетевое взаимодействие
 * 
 * @warning Доступен только на уровне OS, при создании в приложении - UB
 */
class LIBRARIES_EXPORT WebManager_V1
{
private:
    // ======================================== Singleton ========================================

	WebManager_V1() = default;
	~WebManager_V1() = default;
	WebManager_V1(const WebManager_V1&) = delete;
	void operator=(const WebManager_V1&) = delete;

    // ===========================================================================================

	RingFIFO_Static<WebPacket_BufferItem, 32> buffer;			///< Буфер отправляемых сообщений (вычитывается задачей отправки)
	uint32_t buffer_pps{};	///< Packets/sec
	uint32_t buffer_cnt{};	///< Counter
	Timer buffer_sec_timer{ 1000 };

    StaticList<WebConnection_PoolItem, POOL_SIZE> pool;

	StaticList<WebPacket_SyncItem, 8> sync_out;		///< Буфер выходных сообщений с гарантией доставки
	StaticList<WebPacket_SyncItem, 8> sync_in;		///< Буфер входных сообщений с гарантией доставки

    bool FindConnection(WebConnection_PoolItem** connection, uint8_t channel, uint8_t port, uint32_t id);

public:
    static inline WebManager_V1& Instance()
    {
        static WebManager_V1 instance;
        return instance;
    }

	template <typename Tag = void>
	static inline WebManager_V1& TagInstance()
    {
        static WebManager_V1 instance;
        return instance;
    }

	uint32_t BufferCount() const { return buffer.Count(); }

	/// Количество web-пакетов в секунду
	uint32_t BufferPPS() const { return buffer_pps; }

	/** ========================= Функции доступные в LetoAPI_V1 =========================  */

    /**
	 * @brief Создать подключение
	 * 
     * @param[out] connection Открытое подключение
	 * @param[in] channel Канал подключения (COM, nRF, другое)
	 * @param[in] port Порт для подключения
	 * @param[in] id Идентификатор устройства для подключения (при 0 - broadcast)
	 * @param[in] callback Функция обратного вызова при получении данных (может быть NULL)
     * 
     * @return `true` - Подключение успешно создано
	 */
    bool CreateConnection(WebConnection_V1* connection, uint8_t channel, uint8_t port, uint32_t id, WebConnection_V1_Callback callback);

    /**
	 * @brief Закрыть подключение
	 *
	 * @param[in] connection Открытое подключение
	 */
	void CloseConnection (WebConnection_V1 connection);

    /**
	 * @brief Отправить данные без гарантии доставки
	 *
	 * @param[in] connection Открытое подключение
	 * @param[in] data Данные для отправки
	 * @param[in] size Размер данных для отправки
	 */
	void SendData (WebConnection_V1 connection, const void* data, uint32_t size);

    /**
	 * @brief Получить список устройств рядом
	 * 
	 * @param[out] info Массив для получения информации
	 * @param[in] available Размер массива для получения информации
	 *
	 * @return Количество устройств
	 */
	uint32_t GetDevicesNear (WebDeviceInfo_V1* info, uint32_t available) const;

	/**
	 * @brief Найти устройство по идентификатору
	 * 
	 * @param[in] id Идентификатор устройства
	 * @param[out] info Информация об устройстве
	 * 
	 * @return `true` - Устройство успешно найдено
	 */
	bool FindDeviceNear(uint32_t id, WebDeviceInfo_V1* info) const;

	/** ==================================================================================  */

    /**
     * @brief Получить список открытых подключений
     * 
     * @param[out] info Массив для получения информации
	 * @param[in] available Размер массива для получения информации
	 *
	 * @return Количество подключений
     */
    uint32_t GetConnections(WebConnection_V1* info, uint32_t available);

	/**
	 * @brief Отправить данные с гарантией доставки
	 * 
	 * @param[in] connection Открытое подключение
	 * @param[in] data Данные для отправки
	 * @param[in] size Размер данных для отправки
	 */
	void SendSynchronizeData(WebConnection_V1 connection, const void* data, uint32_t size);

	/**
	 * @brief Получить данные
	 * 
	 * @param[in] channel Канал подключения (COM, nRF, другое)
	 * @param[in] port Порт для подключения
	 * @param[in] id Идентификатор устройства для подключения (при 0 - broadcast)
	 * @param[in] data Данные для получения
	 * @param[in] size Размер данных для получения
	 */
	void ReceiveData(uint8_t channel, const WebPacket_V1& packet);

	/**
	 * @brief Проверить наличие пакета для отправки
	 * 
	 * @param[out] item Структура с информацией об отправляемом пакете
	 * 
	 * @return Наличие пакета
	 */
	bool CheckBuffer(WebPacket_BufferItem& info);

	/**
	 * @brief Переключиться на следующий пакет для отправки
	 */
	void NextBuffer();

	/**
	 * @brief Выполнить действия в фоне
	 */
	void Loop();
};


#endif
