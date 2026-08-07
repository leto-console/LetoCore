/*
 * SceneManager.hpp
 *
 *  Created on: Aug 05, 2026
 *      Author: Timur
 */

#ifndef INC_SCENE_MANAGER_SCENE_MANAGER_HPP_
#define INC_SCENE_MANAGER_SCENE_MANAGER_HPP_

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>
#include <SceneManager/ISceneManager.hpp>
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

template <uint32_t TScenesMaxCount, uint32_t TBuilderAllocSize, uint32_t TCommonAllocSize>
class SceneManager : public ISceneManager
{
protected:
	// ======================================== Main part ========================================
    ArenaAllocator<TBuilderAllocSize> builder_allocator;
    ArenaAllocator<TCommonAllocSize> common_allocator;

    const uint32_t scenesMaxCount = TScenesMaxCount;
    ISceneBuilder* sceneBuilders[TScenesMaxCount]{};

	uint32_t currentSceneID{};

	//===========================================================================================

    IAllocator& GetSceneAllocator() override { return builder_allocator; }

	IScene* GetScene(uint32_t ID)
    {
        if (!GetBuilder(ID))
            return nullptr;
        return GetBuilder(ID)->GetObject();
    }
	IScene* GetCurrentScene() {	return GetScene(currentSceneID); }

	ISceneBuilder* GetBuilder(uint32_t ID)
    {
        if (!ID || ID >= scenesMaxCount)
            return nullptr;
        return sceneBuilders[ID];
    }
	ISceneBuilder* GetCurrentBuilder() { return GetBuilder(currentSceneID); }

	uint32_t switch_id{};
	bool switch_return{};

	void OnSceneSwitched()
    {
        if (!switch_id)
            return;

        //printf("OnSceneSwitched: %lu, %s\n", switch_id, switch_return ? "return" : "");

        if (GetBuilder(switch_id))
        {
            if (GetCurrentScene()) 
            {
                GetCurrentScene()->MainOnHide();
                GetCurrentBuilder()->MainDestroy(common_allocator);
            }
            GetBuilder(switch_id)->MainCreate(common_allocator)->MainOnShow();
            if (!switch_return)
                GetBuilder(switch_id)->SetPrevScene(currentSceneID);
            currentSceneID = switch_id;

            //printf("Scene %lu switched to %lu\n", currentSceneID, switch_id);
        }
        else
        {
            //printf("Scene %lu not fount\n", switch_id);
        }

        switch_id = 0;
        switch_return = false;
    }

public:
	using ISceneManager::AddSceneBuilder;
	using ISceneManager::SwitchScene;

    //ISceneManager();
	~SceneManager() = default;

    IAllocator& GetCommonAllocator() override { return common_allocator; }

	void AddSceneBuilder(uint32_t ID, ISceneBuilder* builder) override
    {
        if (!ID || ID >= scenesMaxCount)
            return;
        sceneBuilders[ID] = builder;
    }

	void SwitchScene(uint32_t ID) override
    {
        switch_id = ID;
        switch_return = false;
    }

	/**
	 * @brief Возврат на предыдущую сцену
	 */
	void Return() override
    {
        uint32_t prev_id{};
        
        if (GetCurrentScene() && GetCurrentBuilder()->GetPrevScene(prev_id) && prev_id < scenesMaxCount)
        {
            switch_id = prev_id;
            switch_return = true;
        }
    }

	/// @brief Проверить, существует ли сцена с заданным идентификатор
	/// @param ID Идентификатор проверяемой сцены
	/// @return 
	bool IsExists(uint32_t ID) const override
    {
        if (!ID || ID >= scenesMaxCount)
            return false;
        return sceneBuilders[ID];
    }

	uint32_t GetCurrentSceneID() const override { return currentSceneID; }

	void ClearScenes() override
    {
        for (ISceneBuilder*& _builder : sceneBuilders)
        {
            if (!_builder) continue;
            _builder->MainDestroy(common_allocator);
            _builder = nullptr;
        }

        currentSceneID = 0;
        builder_allocator.Clear();
        common_allocator.Clear();
    }

	void Loop() override
    {
        if (GetCurrentScene()) 	
            GetCurrentScene()->MainLoop();
        
        OnSceneSwitched();
    }

	void Draw(IScreen& screen) override
    {
        screen.ClearScreen();

        if (GetCurrentScene()) 
            GetCurrentScene()->MainDraw(screen);
    }

	bool ProcessEvent(const AppEvent& event) override
    {
        return GetCurrentScene() && GetCurrentScene()->MainProcessInput(event);
    }
};

#endif
