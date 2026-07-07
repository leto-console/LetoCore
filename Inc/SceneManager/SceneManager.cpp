#include <SceneManager/SceneManager.hpp>
#include <Time/TimeUtils.hpp>

#include <stdio.h>
#include <cstring>

#include <Input/ButtonEvent.hpp>
#include <SceneManager/SceneSettings.hpp>
#include <VirtualConsole/VirtualConsole.hpp>

void SceneManager::EnableFPS(bool enable)
{
	enable_fps = enable;
	fps_timer.Start(100);
}

void SceneManager::AddSceneBuilder(uint32_t ID, ISceneBuilder *builder)
{
	if (!ID || ID >= MAIN_COUNT)
		return;
	sceneBuilders[ID] = builder;
}

void SceneManager::SwitchScene(uint32_t ID)
{
	switch_id = ID;
	switch_return = false;
}

void SceneManager::Return()
{
	uint32_t prev_id{};
	
	if (GetCurrentScene() && GetCurrentBuilder()->GetPrevScene(prev_id) && prev_id < MAIN_COUNT)
	{
		switch_id = prev_id;
		switch_return = true;
	}
}

IScene *SceneManager::GetScene(uint32_t ID)
{
	if (!GetBuilder(ID))
		return nullptr;
	return GetBuilder(ID)->GetObject();
}

IScene *SceneManager::GetCurrentScene()
{
	return GetScene(currentSceneID);
}

ISceneBuilder *SceneManager::GetBuilder(uint32_t ID)
{
	if (!ID || ID >= MAIN_COUNT)
		return nullptr;
	return sceneBuilders[ID];
}

ISceneBuilder *SceneManager::GetCurrentBuilder()
{
    return GetBuilder(currentSceneID);
}

void SceneManager::OnSceneSwitched()
{
	if (!switch_id)
		return;

	printf("OnSceneSwitched: %lu, %s\n", switch_id, switch_return ? "return" : "");

	if (GetBuilder(switch_id))
	{
		if (GetCurrentScene()) 
		{
			GetCurrentScene()->MainOnHide();
			GetCurrentBuilder()->MainDestroy(CommonAllocator);
		}
		GetBuilder(switch_id)->MainCreate(CommonAllocator)->MainOnShow();
		if (!switch_return)
			GetBuilder(switch_id)->SetPrevScene(currentSceneID);
		currentSceneID = switch_id;

		printf("Scene %lu switched to %lu\n", currentSceneID, switch_id);
	}
	else
	{
		printf("Scene %lu not fount\n", switch_id);
	}

	switch_id = 0;
	switch_return = false;
}

bool SceneManager::IsExists(uint32_t ID) const
{
	if (!ID || ID >= MAIN_COUNT)
		return false;
	return sceneBuilders[ID];
}

void SceneManager::ClearScenes()
{
	for (ISceneBuilder*& _builder : sceneBuilders)
	{
		if (!_builder) continue;
		_builder->MainDestroy(CommonAllocator);
		_builder = nullptr;
	}

	currentSceneID = 0;
}

bool SceneManager::Loop()
{
	bool screen_ok{}, scene_ok{};

	if (GetCurrentScene()) 	
		scene_ok = GetCurrentScene()->MainLoop();

	if (menu_holder.Holded(200, ButtonHoldHandler::OnHoldPolicy::KEEP))
	{
		EnableFPS_Setting.Set(false);
		VirtualConsole::Instance().Enable();
	}
	else
	{
		VirtualConsole::Instance().Disable();
		VirtualConsole::Instance().ResetViewed();
	}
	
	OnSceneSwitched();

	return screen_ok && scene_ok;
}

#include <Graphics/DefaultFont.hpp>
#include <DrawFunctions/DrawText.hpp>

/// TODO: Оформить в более приятный объекты

static void DrawCapacity(IScreen& screen, Point2_i& p, StaticText8 letter, CapacityIndicatorUI& ind, IAllocator& allocator)
{
	static StaticText32 text{};
	DrawFunctions::DrawText(screen, p, letter, WhiteColor, BlackColor, false, &Default_Font_7x7_small);
	p.x += 8;
	ind.SetPosition(p + Point2_i{0, 1});
	snprintf(text.CharPtr(), text.Capacity(), "%d", static_cast<int>(allocator.GetPercentage() * 100));
	DrawFunctions::DrawText(screen, p - Point2_i{0, 7}, text, WhiteColor, BlackColor, false, &Default_Font_7x7_small);
	p.x += (ind.GetWidth() + 2);
	ind.Draw(screen);
}

void SceneManager::Draw(IScreen& screen)
{
	screen.ClearScreen();

	if (GetCurrentScene()) 
		GetCurrentScene()->MainDraw(screen);

	VirtualConsole::Instance().MainDraw(screen);

	if (enable_fps)
	{
		frames_count++;
		if (fps_timer.Expired())
		{
			average_fps.Push(frames_count);
			frames_count = 0;
			fps_timer.Start();
		}

		static StaticText32 text{};
		snprintf(text.CharPtr(), text.Capacity(), "%d", average_fps.Sum());
		DrawFunctions::DrawText(screen, {0, 64 - 8}, text, WhiteColor, BlackColor, false, &Default_Font_7x7_small);

		Point2_i p = { 20, 64-8 };

		DrawCapacity(screen, p, "S", capacity1, SystemAllocator);
		DrawCapacity(screen, p, "C", capacity2, CommonAllocator);
		DrawCapacity(screen, p, "E", capacity3, EternalAllocator);
    }
}

#include <Input/SystemInputID.hpp>
#include "SceneManager.hpp"

bool SceneManager::ProccessUserInput(const AppEvent& event)
{
	// Обработка системного меню
	if (IsSystemMenuEvent(event))
	{
		EnableFPS_Setting.Set(!enable_fps);
		menu_holder.Press();
		return true;
	}
	else if (IsSystemMenuEvent(event, false))
	{
		menu_holder.Release();
		return true;
	}

	if (VirtualConsole::Instance().MainProcessInput(event))
		return true;

	if (event.source == AE_Encoder)
		last_encoder_timer.Start(100);
	else if (!last_encoder_timer.Expired(false) && !ButtonEvent::IsReleased(event))
		return false;

	return GetCurrentScene() && GetCurrentScene()->MainProcessInput(event);
}
