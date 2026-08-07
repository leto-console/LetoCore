/*
 * SystemSceneManager.hpp
 *
 *  Created on: Oct 19, 2025
 *      Author: Timur
 */

#ifndef INC_SCENE_MANAGER_SYSTEM_SCENE_MANAGER_HPP_
#define INC_SCENE_MANAGER_SYSTEM_SCENE_MANAGER_HPP_

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>
#include <System/EternalAllocator.hpp>
#include <LetoABI/AppEvent.h>
#include <Time/Timer.hpp>
#include <Input/Catchers/ButtonCatcher.hpp>

#include <type_traits>
#include <utility>

#include <Data/RingFIFO_Static.hpp>
#include <UI/CapacityIndicatorUI.hpp>

#include <Graphics/IDrawer.hpp>
#include <GamesSupport/GameCenter.hpp>
#include <TaskHandler/PriorityTaskSheduler.hpp>

#include <SceneManager/SceneManager.hpp>

class LETO_CORE_EXPORT SystemSceneManager : public SceneManager<32, 1024, 20 * 1024>
{
protected:

	// ======================================== Singleton ========================================

	SystemSceneManager();
	~SystemSceneManager() = default;
	SystemSceneManager(const SystemSceneManager&) = delete;
	void operator=(const SystemSceneManager&) = delete;

	// ===========================================================================================

	// ======================================== FPS part ========================================

	Timer fps_timer{};
	RingFIFO_Static<short, 10> average_fps;
	short frames_count = 0;
	bool enable_fps{};
	ButtonCatcher<SystemSceneManager> menu_hold_catcher;	/// Обработчик зажатия системной клавиши Меню

	//===========================================================================================
	void OnMenuHolded();

public:
	static inline SystemSceneManager& Instance()
	{
		static SystemSceneManager instance;
		return instance;
	}

	void EnableFPS(bool enable = true);

	void Loop() override;
	void Draw(IScreen& screen) override;
	bool ProcessEvent(const AppEvent& event) override;
};

#endif
