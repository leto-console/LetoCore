#include <SceneManager/SystemSceneManager.hpp>
#include <Time/TimeUtils.hpp>

#include <stdio.h>
#include <cstring>

#include <Input/ButtonEvent.hpp>
#include <SceneManager/SystemSceneSettings.hpp>
#include <VirtualConsole/VirtualConsole.hpp>
#include <Input/SystemInputID.hpp>

void SystemSceneManager::EnableFPS(bool enable)
{
	enable_fps = enable;
	fps_timer.Start(100);
}

SystemSceneManager::SystemSceneManager() :
	menu_hold_catcher{ this, &SystemSceneManager::OnMenuHolded },
	menu_double_click_catcher{ this, &SystemSceneManager::OnMenuDoubleClick }
{
	menu_hold_catcher.Catch(SYSTEM_BTN_MENU, BCM_HOLD);
	menu_hold_catcher.SetHoldTime(200);
	menu_hold_catcher.Enable();

	menu_double_click_catcher.Catch(SYSTEM_BTN_MENU, BCM_DOUBLE_CLICK);
	menu_double_click_catcher.Enable();

	VirtualConsole::Instance().Disable();
	VirtualConsole::Instance().ResetViewed();
}

void SystemSceneManager::OnMenuHolded()
{
	EnableFPS_Setting.Set(false);
	VirtualConsole::Instance().Enable();
}

#include <AppLoader/AppLoader.hpp>

void SystemSceneManager::OnMenuDoubleClick()
{
	UnloadApplication();
}

void SystemSceneManager::Loop()
{
	menu_hold_catcher.MainLoop();
	menu_double_click_catcher.MainLoop();
	SceneManager::Loop();
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

void SystemSceneManager::Draw(IScreen& screen)
{
	SceneManager::Draw(screen);

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

		{
			IAllocator* allocator = &SystemAllocator;
			StaticText8 letter = "S";

			static CapacityIndicatorUI capacity{{}, {20, 5}, allocator};
			DrawCapacity(screen, p, letter, capacity, *allocator);
		}
		{
			IAllocator* allocator = &common_allocator;
			StaticText8 letter = "C";

			static CapacityIndicatorUI capacity{{}, {20, 5}, allocator};
			DrawCapacity(screen, p, letter, capacity, *allocator);
		}
		{
			IAllocator* allocator = &EternalAllocator;
			StaticText8 letter = "E";

			static CapacityIndicatorUI capacity{{}, {20, 5}, allocator};
			DrawCapacity(screen, p, letter, capacity, *allocator);
		}
		{
			IAllocator* allocator = &builder_allocator;
			StaticText8 letter = "B";

			static CapacityIndicatorUI capacity{{}, {20, 5}, allocator};
			DrawCapacity(screen, p, letter, capacity, *allocator);
		}
    }
}

bool SystemSceneManager::ProcessEvent(const AppEvent& event)
{
	menu_hold_catcher.MainProcessInput(event);
	menu_double_click_catcher.MainProcessInput(event);
	
	// Обработка системного меню
	if (IsSystemMenuEvent(event))
	{
		EnableFPS_Setting.Set(!enable_fps);
		return true;
	}
	else if (IsSystemMenuEvent(event, false))
	{
		VirtualConsole::Instance().Disable();
		VirtualConsole::Instance().ResetViewed();
		return true;
	}

	if (VirtualConsole::Instance().MainProcessInput(event))
		return true;

	return SceneManager::ProcessEvent(event);
}
