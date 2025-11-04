/**
 * @file Timer.hpp
 * @date Dec 8, 2025
 * @author Rakhimov T.
 */

#ifndef INC_TIME_TIMER_HPP_
#define INC_TIME_TIMER_HPP_

#include "LibrariesExport.h"

#include <cstdint>

class LIBRARIES_EXPORT Timer
{
public:
	Timer(uint32_t period_ms = 0);

	// Запустить таймер с последним заданными периодом
	void Start();

	// Запустить таймер с заданным периодом
	void Start(uint32_t period_ms);

	// Получить долю оставшегося времени от start до наступления period_ms (при bound = true, верхняя граница 1.0)
	float GetProgress(bool bound = true) const;

	// Истечение времени таймера (при once = true флаг истечения будет выдан один раз до следущего запуска)
	bool Expired(bool once = true) const;

protected:
	// Время запуска таймера
	uint32_t start_ms{};

	// Период работы таймера
	uint32_t period_ms{};

	// Возвращался ли текущий результат истечения времени таймера (для once)
	mutable bool return_expired{};
};

#endif