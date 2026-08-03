/**
 * @file PriorityTaskSheduler.hpp
 * @date Nov 11, 2025
 * @author Rakhimov T.
 */

#ifndef INC_TASK_HANDLER_PRIORITY_TASK_SHEDULER_H_
#define INC_TASK_HANDLER_PRIORITY_TASK_SHEDULER_H_

#include "LibrariesExport.h"

#include <stdint.h>
#include <algorithm>

#include <TaskHandler/PriorityTask.hpp>
#include <Data/ArenaAllocator.hpp>
#include <Data/StaticText.hpp>
#include <Data/StaticList.hpp>
#include <Data/StaticListView.hpp>
#include <System/SystemAllocator.hpp>

#include <utility>

/// @brief Планировщик задач по приоритетам и периодам выполнения
class LETO_CORE_EXPORT PriorityTaskSheduler
{
protected:
	/// @brief Добавить задачу
	/// @param task Указатель на экземпляр задачи
	static void AddTask(PriorityTask* task);

public:
	template <typename Task, typename... Args>
	static Task* AddTask(const StaticText32& name, uint32_t period_ms, uint8_t priority, Args... args)
	{
		static_assert(std::is_base_of<PriorityTask, Task>::value);
		Task* task = SystemAllocator.Make<Task>(name, period_ms, priority, std::forward<Args>(args)...);
		AddTask(task);
		return task;
	}

	/// @brief Получить список задач
	static StaticListView<PriorityTask*> GetList();

	/// @brief Удалить все задачи из планировщика
	static void Clear();

	/**
	 * @brief Функция обработки задач
	 * 
	 * Необходимо вызывать в основном цикле
	 */
	static bool Shedule();
};

#endif
