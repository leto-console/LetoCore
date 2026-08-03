/*
 * UserInputDevice.hpp
 *
 *  Created on: Nov 04, 2025
 *      Author: Timur
 */

#ifndef INC_INPUT_USER_INPUT_DEVICE_HPP_
#define INC_INPUT_USER_INPUT_DEVICE_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <Data/RingFIFO_Static.hpp>
#include <LetoABI/AppEvent.h>

#ifndef USE_HAL_DRIVER
#include <list>
#endif

#ifdef _WIN32
#include <GraphicsPC/WinDisplay.hpp>
#endif

// Интерфейс устройства пользовательского ввода
class LETO_CORE_EXPORT UserInputDevice
{
protected:
#ifndef USE_HAL_DRIVER
	static std::list<uint64_t> keys;
	
	bool KeyExists(uint64_t key);
#endif

	static RingFIFO_Static<AppEvent, 64> events;	// Кольцевой буфер с событиями ввода

	const uint16_t source;				// Источник пользовательского ввода

	uint16_t id{};						// Идентификатор источника ввода (номер кнопки, энкодера и т.д.)
	bool initialized = false;			// Флаг инициализации

	void PushEvent(uint32_t data = 0)
	{
		if (!initialized)
			return;
		events.Push({ source, id, data });
	}

public:
	UserInputDevice(uint16_t source) : source { source }
	{ }

	virtual ~UserInputDevice() = default;

	// Периодически вызываемая функция для обработки
	virtual void Tick() = 0;

	void Init(uint16_t _id)
	{
		this->id = _id;
		initialized = true;
	}

	// Функция получения события
	static bool PopEvent(AppEvent& event)
	{
		return events.Pop(event);
	}
};

#endif
