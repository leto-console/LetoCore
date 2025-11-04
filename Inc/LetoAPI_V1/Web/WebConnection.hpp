/**
 * @file WebConnection.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_CONNECTION_HPP_
#define INC_LETO_API_V1_WEB_WEB_CONNECTION_HPP_

#include <cstdint>

const uint8_t WC_CHANNEL_ALL 	= 0x00;				///< Подключение со всеми
const uint8_t WC_CHANNEL_UART 	= 0x01;				///< Подключение по UART
const uint8_t WC_CHANNEL_NRF 	= 0x02;				///< Подключение по nRF
const uint8_t WC_CHANNEL_MAX 	= WC_CHANNEL_NRF;	///< Максимальное разнообразие каналов

const uint8_t WC_PORT_PING		= 0x01;		///< Порт для пинга
const uint8_t WC_PORT_INFO		= 0x02;		///< Информационный порт (heartbeat)
const uint8_t WC_PORT_SYSTEM 	= 0x20;		///< Начальный порт системных портов (0x20...0x60 - 64 порта)
const uint8_t WC_PORT_INF_LOBBY	= 0x70;		///< Порт для поиска игровых комнат и обмена информацией
const uint8_t WC_PORT_CON_LOBBY	= 0x77;		///< Порт для взаимодействия в игровых комнатах
const uint8_t WC_PORT_APP 		= 0x80;		///< Начальный порт взаимодействия приложений (0x80...0xC0 - 64 порта)


#pragma pack(push, 4)

/**
 * @brief Callback для обработки получения данных
 *
 * @param[in] channel Канал отправителя
 * @param[in] port Порт отправителя
 * @param[in] id Идентификатор отправителя
 * @param[in] data Данные для получения
 * @param[in] size Размер данных для получения
 */
using WebConnection_V1_Callback = void (*)(uint8_t channel, uint8_t port, uint32_t id, const void* data, uint32_t size);

/**
 * @brief Структура с данными о подключении
 */
struct WebConnection_V1
{
	/// Канал подключения (COM, nRF, другое) (при 0 - все)
	uint8_t channel;
	
	/// Порт для подключения (системное сообщение, игровое сообщение и т.д.)
	uint8_t port;
	
	/// Идентификатор устройства, с которым взаимодействуем (при 0 - broadcast)
	uint32_t id;
	
	/// Callback для обработки получения данных
	WebConnection_V1_Callback callback;

	/// Резерв для возможности расширить функцию
	uint32_t _reserved_[5];
};

#pragma pack(pop)

#endif /* INC_LETO_API_V1_WEB_WEB_CONNECTION_HPP_ */
