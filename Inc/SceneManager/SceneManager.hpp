/*
 * SceneManager.hpp
 *
 *  Created on: Oct 19, 2025
 *      Author: Timur
 */

#ifndef INC_SCENE_MANAGER_SCENE_MANAGER_HPP_
#define INC_SCENE_MANAGER_SCENE_MANAGER_HPP_

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>
#include <System/CommonAllocator.hpp>
#include <System/EternalAllocator.hpp>
#include <ABI/AppEvent.hpp>
#include <Time/Timer.hpp>
#include <Input/Catchers/ButtonCatcher.hpp>

#include <type_traits>
#include <utility>

#include <Data/RingFIFO_Static.hpp>
#include <UI/CapacityIndicatorUI.hpp>

#include <Graphics/IDrawer.hpp>
#include <GamesSupport/GameCenter.hpp>
#include <TaskHandler/PriorityTaskSheduler.hpp>

class LETO_CORE_EXPORT SceneManager : public IDrawer
{
protected:

	// Антидребезг энкодера
	Timer last_encoder_timer{};

	// (TEMP) Отрисовка занятости аллокаторов
	CapacityIndicatorUI capacity1{{}, {20, 5}, &SystemAllocator};
	CapacityIndicatorUI capacity2{{}, {20, 5}, &CommonAllocator};
	CapacityIndicatorUI capacity3{{}, {20, 5}, &EternalAllocator};

	// ======================================== Singleton ========================================

	SceneManager();
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	void operator=(const SceneManager&) = delete;

	// ===========================================================================================

	// ======================================== Main part ========================================

	uint32_t currentSceneID{};
	enum { MAIN_COUNT = 32 };
	ISceneBuilder* sceneBuilders[MAIN_COUNT]{};

	//===========================================================================================

	// ======================================== FPS part ========================================

	Timer fps_timer{};
	RingFIFO_Static<short, 10> average_fps;
	short frames_count = 0;
	bool enable_fps{};
	ButtonCatcher<SceneManager> menu_hold_catcher;	/// Обработчик зажатия системной клавиши Меню

	//===========================================================================================

	IScene* GetScene(uint32_t ID);
	IScene* GetCurrentScene();

	ISceneBuilder* GetBuilder(uint32_t ID);
	ISceneBuilder* GetCurrentBuilder();

	uint32_t switch_id{};
	bool switch_return{};

	void OnMenuHolded();
	void OnSceneSwitched();

public:
	static inline SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void EnableFPS(bool enable = true);

	void AddSceneBuilder(uint32_t ID, ISceneBuilder* builder);

	template <typename Scene, typename ID, typename... Args>
	void AddSceneBuilder(ID id, Args... args)
	{
		static_assert(std::is_base_of<ISceneBuilder, typename Scene::Builder>::value);
		ISceneBuilder* builder =  SystemAllocator.Make<typename Scene::Builder>(std::forward<Args>(args)...);
		AddSceneBuilder((uint32_t) id, builder);
	}

	template <typename ID>
	void SwitchScene(ID id)
	{
		SwitchScene((uint32_t)id);
	}
	void SwitchScene(uint32_t ID);

	/**
	 * @brief Возврат на предыдущую сцену
	 */
	void Return();

	/// @brief Проверить, существует ли сцена с заданным идентификатор
	/// @param ID Идентификатор проверяемой сцены
	/// @return 
	bool IsExists(uint32_t ID) const;

	uint32_t GetCurrentSceneID() const { return currentSceneID; }

	void ClearScenes();

	bool Loop() override;
	void Draw(IScreen& screen) override;
	bool ProccessUserInput(const AppEvent& event);
};

#endif
