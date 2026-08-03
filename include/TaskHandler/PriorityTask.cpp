#include <TaskHandler/PriorityTask.hpp>

#include <Time/TimeUtils.hpp>

bool PriorityTask::Ready() const
{
	return TimeUtils::GetCurrentMks() > end_mks + period_ms * 1000;
}

void PriorityTask::TaskDo()
{
	if (start_mks <= end_mks)
		start_mks = TimeUtils::GetCurrentMks();

	if (Do())
	{
		end_mks = TimeUtils::GetCurrentMks();
		last_durations_ms.Add((end_mks - start_mks) / 1000.0f);
	}
}
