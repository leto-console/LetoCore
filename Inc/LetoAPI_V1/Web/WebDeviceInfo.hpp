/**
 * @file WebDeviceInfo.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_DEVICE_INFO_HPP_
#define INC_LETO_API_V1_WEB_WEB_DEVICE_INFO_HPP_

#include <cstdint>

#pragma pack(push, 4)

/**
 * @brief Информация об устройстве в сети
 */
struct WebDeviceInfo_V1
{
	/// Канал подключения (COM, nRF, другое)
	uint8_t channel;

	/// Идентификатор
	uint32_t id;
		
	/// Идентификатор аватарки
	uint32_t avatar_id;

	/// Идентификатор открытого приложения
	uint32_t app_id;
	
	/// Сетевое имя (UTF-8)
	char web_name[32];

	/// Наличие подключение к лобби
	uint32_t lobby_owner;

	/// Запрос на подключение к лобби
	uint32_t joining_lobby;
};

#pragma pack(pop)

#endif /* INC_LETO_API_V1_WEB_WEB_DEVICE_INFO_HPP_ */
