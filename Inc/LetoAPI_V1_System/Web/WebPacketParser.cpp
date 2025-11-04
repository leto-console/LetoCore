#include "WebPacketParser.hpp"

#include <LetoAPI_V1_System/Web/WebPacket.hpp>
#include <LetoAPI_V1_System/Web/WebManager.hpp>

#include <cstring>

static uint8_t buffer[WC_CHANNEL_MAX][sizeof(WebPacket_V1) * 2]{};
static uint8_t current_byte[WC_CHANNEL_MAX]{};

static void Reset(uint8_t channel)
{
	// if (channel >= WC_CHANNEL_MAX) return;
	current_byte[channel] = 0;
}

void WebPacketParser::CallbackByte(uint8_t channel, uint8_t byte)
{
	static WebPacket_V1 example_command;
	static uint8_t* example_command_raw = (uint8_t*)&example_command;

	if (channel >= WC_CHANNEL_MAX)
		return;

	// Первые 2 байта контрольные
	if (current_byte[channel] >= 0 && current_byte[channel] < sizeof(example_command.__magic))
	{
		if (byte != example_command_raw[current_byte[channel]])
		{
			Reset(channel);
			return;
		}
	}

	buffer[channel][current_byte[channel]++] = byte;

	// Если текущее количество байт равно размеру команды, отправляем её в кольцевой буфер команд
	if (current_byte[channel] == sizeof(WebPacket_V1))
	{
		WebPacket_V1 msg_in{};
		memcpy(reinterpret_cast<void*>(&msg_in), buffer[channel], sizeof(WebPacket_V1));
		if (msg_in.CheckCRC16())
		{
            WebManager_V1::Instance().ReceiveData(channel, msg_in);
		}
		Reset(channel);
		return;
	}
}
