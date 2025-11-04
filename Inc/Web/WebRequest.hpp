/*
 * WebRequest.hpp
 *
 *  Created on: Dec 26, 2025
 *      Author: Timur
 */

#ifndef INC_WEB_WEB_REQUEST_HPP_
#define INC_WEB_WEB_REQUEST_HPP_

#include "LibrariesExport.h"

#include <stdint.h>

#include <LetoAPI_V1_System/Web/WebPacket.hpp>

// Объект, отвечающий за отправку запросов и формирование ответов на запросы
class LIBRARIES_EXPORT WebRequest
{
protected:
	// Тип запроса
	const uint8_t request_type{};

	// Байты типов запроса и ответа в WebPacket_V1
	const uint8_t request_byte{}, response_byte{};

	// В ожидании ответа
	bool waiting_response : 1;

	// В процессе обработки запроса
	bool processing_request : 1;

public:
	WebRequest(uint8_t request_type, uint8_t request_byte = 2, uint8_t response_byte = 3);

	// Отменить ожидание ответа или обработку запроса
	void Cancel();

	// Запрос в ожидании ответа
	bool IsWaitingResponse() const;

	// Запрос ожидает обработки
	bool IsProcessingRequest() const;

	// Проверка на соответствие форме запроса
	bool IsRequest(const WebPacket_V1& msg) const;

	// Проверка на соответствие форме ответа
	bool IsResponse(const WebPacket_V1& msg) const;

	// Проверка на соответствие форме ответа
	bool IsResponse(const WebPacket_V1& msg, uint8_t& response_type) const;

	// Отправить запрос
	void MakeRequest(WebPacket_V1& msg_out);

	// Проверить и обработать запрос
	bool ProcessRequest(const WebPacket_V1& msg_in);

	// Отправить ответ (можно с тем же сообщением, можно отправить с другим содержанием)
	void MakeResponse(WebPacket_V1& msg_out, uint8_t response_type);

	// Проверить и обработать ответ (только при наличии ожидания ответа)
	bool ProcessResponse(const WebPacket_V1& msg_in);

	// Проверить и обработать ответ (только при наличии ожидания ответа)
	bool ProcessResponse(const WebPacket_V1& msg_in, uint8_t& response_type);
};

#endif
