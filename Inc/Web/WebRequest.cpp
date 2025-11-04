#include <Web/WebRequest.hpp>

WebRequest::WebRequest(uint8_t request_type, uint8_t request_byte, uint8_t response_byte) :
	request_type{ request_type }, request_byte{ request_byte }, response_byte{ response_byte },
	waiting_response{ false }, processing_request{ false }
{
}

void WebRequest::Cancel()
{
	waiting_response = processing_request = false;
}

bool WebRequest::IsWaitingResponse() const
{
	return waiting_response;
}

bool WebRequest::IsProcessingRequest() const
{
	return processing_request;
}

bool WebRequest::IsRequest(const WebPacket_V1& msg) const
{
	return msg.data[request_byte /*2*/] == request_type && msg.data[response_byte /*3*/] == 0;
}

bool WebRequest::IsResponse(const WebPacket_V1& msg) const
{
	uint8_t r;
	return IsResponse(msg, r);
}

bool WebRequest::IsResponse(const WebPacket_V1& msg, uint8_t& response_type) const
{
	response_type = msg.data[response_byte /*3*/];
	return msg.data[request_byte /*2*/] == request_type && response_type != 0;
}

void WebRequest::MakeRequest(WebPacket_V1& msg_out)
{
	waiting_response = true;
	// Тип запроса
	msg_out.data[request_byte /*2*/] = request_type;
	// Поле с ответом пустое
	msg_out.data[response_byte /*3*/] = 0;
}

bool WebRequest::ProcessRequest(const WebPacket_V1& msg_in)
{
	bool is_request = IsRequest(msg_in);
	if (is_request)
		processing_request = true;
	return is_request;
}

void WebRequest::MakeResponse(WebPacket_V1& msg_out, uint8_t response_type)
{
	processing_request = false;
	// Если тип ответа == 0, ответ будет считаться запросом, поэтому такие ответы не отправляем
	if (response_type == 0)
		return;
	// Запрос сохраняется
	msg_out.data[request_byte /*2*/] = request_type;
	// Формируется поле с ответом
	msg_out.data[response_byte /*3*/] = response_type;
}

bool WebRequest::ProcessResponse(const WebPacket_V1& msg_in)
{
	uint8_t r;
	return ProcessResponse(msg_in, r);
}

bool WebRequest::ProcessResponse(const WebPacket_V1& msg_in, uint8_t& response_type)
{
	// Исключает двойную обработку ответа на запрос (если не ждем, считаем, что ответа на запрос и нет)
	if (waiting_response)
	{
		bool is_response = IsResponse(msg_in, response_type);
		if (is_response)
			waiting_response = false;
		return is_response;
	}
	return false;
}
