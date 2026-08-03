/*
 * PriorityTask.hpp
 *
 *  Created on: Nov 11, 2025
 *      Author: Timur
 */

#ifndef INC_TASK_HANDLER_PRIORITY_TASK_H_
#define INC_TASK_HANDLER_PRIORITY_TASK_H_

#include "LibrariesExport.h"

#include <stdint.h>
#include <Data/StaticText.hpp>
#include <Data/MovingAverage.hpp>

/**
 * @brief Периодически выполняемая задача с приоритетом
 */
class LETO_CORE_EXPORT PriorityTask
{
protected:
	StaticText32 name;		///< Название задачи
	uint32_t period_ms;		///< Период выполнения задачи в мс
	uint8_t priority;		///< Приоритет выполнения задачи (чем выше, тем приоритетнее)

	/// @brief Скользящее среднее с продолжительностью выполнения задачи в мс
	MovingAverage<float, 8> last_durations_ms;

	// Начало и конец работы задачи в мкс
	uint32_t start_mks{};
	uint32_t end_mks{};		// Последний запуск задачи

	/**
	 * @brief Выполняемая с заданным периодом и приоритетом функция
	 * 
	 * Вызывается при готовности выполнения задачи `PriorityTask::Ready()`
	 * 
	 * @return Завершение выполнения задачи (при возвращении `false` задача становится фрагментированной)
	 */
	virtual bool Do() = 0;

public:
	PriorityTask(const StaticText32& name, uint32_t period_ms, uint8_t priority)
		: name { name }, period_ms{ period_ms }, priority{ priority }
	{ }

	virtual ~PriorityTask() = default;

	StaticText32 GetName() const { return name; }
	uint32_t GetPeriodMs() const { return period_ms; }
	uint8_t GetPriority() const { return priority; }
	float GetAverageDurationMs() const { return last_durations_ms.Get(); }

	/**
	 * @brief Флаг готовности выполнения задачи
	 * 
	 * Вычисляется на основании периода
	 */
	bool Ready() const;

	/**
	 * @brief Выполнить задачу с подсчетом времени выполнения
	 */
	void TaskDo();
};

#endif
