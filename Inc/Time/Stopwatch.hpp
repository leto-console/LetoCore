/**
 * @file Stopwatch.hpp
 * @date Jul 19, 2026
 * @author Rakhimov T.
 */

#ifndef INC_TIME_STOPWATCH_HPP_
#define INC_TIME_STOPWATCH_HPP_

#include "LibrariesExport.h"

#include <cstdint>

class LIBRARIES_EXPORT Stopwatch
{
public:
	Stopwatch(bool auto_start = false);

	// Запустить таймер с заданным периодом
	void Start();

    // Получить прошедшее со старта количество миллисекунд
	uint32_t ElapsedMs();

    // Получить прошедшее со старта количество микросекунд
	uint32_t ElapsedMks();

protected:
	// Время запуска таймера, мкс
	uint32_t start_mks{};
};

#endif