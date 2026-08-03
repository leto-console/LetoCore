/*
 * FunctionTask.cpp
 *
 *  Created on: Nov 13, 2025
 *      Author: Timur
 */

#include <TaskHandler/PriorityTaskSheduler.hpp>
#include "PriorityTaskSheduler.hpp"

static StaticList<PriorityTask*, 32> TaskList;

void PriorityTaskSheduler::AddTask(PriorityTask *task)
{
    // Не добавляется задача с таким же именем
    for (PriorityTask* _task : TaskList)
        if (_task->GetName() == task->GetName())
            return;

    TaskList.Push(task);

    std::sort(
        TaskList.begin(), TaskList.end(),
        [](PriorityTask* left, PriorityTask* right) -> bool
        {
            return left->GetPriority() > right->GetPriority();
        }
    );
}

StaticListView<PriorityTask*> PriorityTaskSheduler::GetList()
{
    return TaskList;
}

void PriorityTaskSheduler::Clear()
{
    for (PriorityTask* _task : TaskList)
        _task->~PriorityTask();

    TaskList.Clear();
}

bool PriorityTaskSheduler::Shedule()
{
    for (PriorityTask* task : TaskList)
    {
        if (!task->Ready())
            continue;
        task->TaskDo();
        return true;
    }
    return false;
}
