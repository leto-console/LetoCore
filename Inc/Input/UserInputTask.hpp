/*
 * UserInputTask.hpp
 *
 *  Created on: Nov 13, 2025
 *      Author: Timur
 */

#ifndef INC_INPUT_USER_INPUT_TASK_HPP_
#define INC_INPUT_USER_INPUT_TASK_HPP_

#include <TaskHandler/PriorityTask.hpp>
#include <SceneManager/SceneManager.hpp>
#include <Data/StaticListView.hpp>
#include <Input/Devices/UserInputDevice.hpp>

class UserInputTickTask : public PriorityTask
{
protected:
	StaticListView<UserInputDevice*> user_inputs;

	bool Do() override
	{
		for (UserInputDevice* user_input : user_inputs)
			user_input->Tick();
		return true;
	}

public:
	UserInputTickTask(const StaticText32& name, uint32_t period_ms, uint8_t priority, StaticListView<UserInputDevice*> user_inputs)
		: PriorityTask{ name, period_ms, priority }, user_inputs{ user_inputs }
	{
	}
};

class UserInputPopEventTask : public PriorityTask
{
protected:
	StaticListView<UserInputDevice*> user_inputs;

	bool Do() override
	{
		AppEvent event;

		for (UserInputDevice* user_input : user_inputs)
		{
			while (user_input->PopEvent(event))
			{
//				printf("AppEvent:\n");
//				printf("event.source=%d\n", event.source);
//				printf("event.id=%d\n", event.id);
//				printf("event.data=%d\n", event.data);

				SceneManager::Instance().ProccessUserInput(event);
			}
		}
		return true;
	}

public:
	UserInputPopEventTask(const StaticText32& name, uint32_t period_ms, uint8_t priority, StaticListView<UserInputDevice*> user_inputs)
		: PriorityTask{ name, period_ms, priority }, user_inputs{ user_inputs }
	{
	}
};

#endif
