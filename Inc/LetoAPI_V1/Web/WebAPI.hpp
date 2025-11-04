/**
 * @file WebAPI_V1.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_API_HPP_
#define INC_LETO_API_V1_WEB_WEB_API_HPP_

#include <cstdint>
#include <type_traits>

#include <LetoAPI_V1/Web/WebConnection.hpp>
#include <LetoAPI_V1/Web/WebDeviceInfo.hpp>

#pragma pack(push, 4)

/// Функции для сетевого взаимодейтствия
struct WebAPI_V1
{
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
	bool (*CreateConnection) (WebConnection_V1* connection, uint8_t channel, uint8_t port, uint32_t id, WebConnection_V1_Callback callback);
	
	/**
	 * @brief Закрыть подключение
	 *
	 * @param[in] connection Открытое подключение
	 */
	void (*CloseConnection) (WebConnection_V1 connection);
	
	/**
	 * @brief Отправить данные
	 *
	 * @param[in] connection Открытое подключение
	 * @param[in] data Данные для отправки
	 * @param[in] size Размер данных для отправки
	 */
	void (*SendData) (WebConnection_V1 connection, const void* data, uint32_t size);
	
	/**
	 * @brief Получить список устройств рядом
	 * 
	 * @param[out] info Массив для получения информации
	 * @param[in] available Размер массива для получения информации
	 *
	 * @return Количество устройств
	 */
	uint32_t (*GetDevicesNear) (WebDeviceInfo_V1* info, uint32_t available);

	/**
	 * @brief Найти устройство по идентификатору
	 * 
	 * @param[in] id Идентификатор устройства
	 * @param[out] info Информация об устройстве
	 * 
	 * @return `true` - Устройство успешно найдено
	 */
	bool (*FindDeviceNear)(uint32_t id, WebDeviceInfo_V1* info);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<WebAPI_V1>::value, "WebAPI_V1 должен быть standard_layout");

#endif
