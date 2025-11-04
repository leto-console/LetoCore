/**
 * @file IDisplayTasks.hpp
 * @date 05 Apr, 2026
 */

#ifndef INC_GRAPHICS_IDISPLAY_TASKS_HPP_
#define INC_GRAPHICS_IDISPLAY_TASKS_HPP_

#include <TaskHandler/PriorityTask.hpp>

#include <SceneManager/SceneManager.hpp>
#include <Data/StaticText.hpp>
#include <Graphics/IDisplay.hpp>

#include <stdint.h>

class IDisplay_RenderTask : public PriorityTask
{
protected:
	IDisplay* display;

    // Стадия отправки изображения на экран
    bool render_stage{};

	bool Do() override
    {
		return display->Render(&SceneManager::Instance());
    }

public:
	IDisplay_RenderTask(const StaticText32& name, uint32_t period_ms, uint8_t priority, IDisplay* display)
    	: PriorityTask{ name, period_ms, priority }, display{ display }
    {
    }

};

class IDisplay_LoopTask : public PriorityTask
{
protected:
	IDisplay* display;

	bool Do() override
    {
		display->Loop();
        SceneManager::Instance().Loop();
        return true;
    }

public:
	IDisplay_LoopTask(const StaticText32& name, uint32_t period_ms, uint8_t priority, IDisplay* display)
    	: PriorityTask{ name, period_ms, priority }, display{ display }
    {
    }

};

#endif
