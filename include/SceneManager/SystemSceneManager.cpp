#include <SceneManager/SystemSceneManager.hpp>
#include <Time/TimeUtils.hpp>

#include <stdio.h>
#include <cstring>

#include <Input/ButtonEvent.hpp>
#include <SceneManager/SystemSceneSettings.hpp>
#include <VirtualConsole/VirtualConsole.hpp>
#include <Input/SystemInputID.hpp>
#include <System/DebugMode.hpp>

void SystemSceneManager::EnableFPS(bool enable)
{
	enable_fps = enable;
	fps_timer.Start(100);
	SetDebugMode(enable);
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

		// TODO:: support for SSD1306
		Point2_i p = { 0, 128-8 };

		static StaticText32 text{};
		snprintf(text.CharPtr(), text.Capacity(), "%d", average_fps.Sum());
		DrawFunctions::DrawText(screen, p, text, WhiteColor, BlackColor, false, &Default_Font_7x7_small);

		p.x += 24;

		Point2_i offsets[4]
		{
			{0,0},
			{30,0},
			{60,0},
			{90,0},
		};

		{
			static CapacityIndicatorUI capacity{p + offsets[0], {20, 5}, &SystemAllocator, "S"};
			capacity.Draw(screen);
		}
		{
			static CapacityIndicatorUI capacity{p + offsets[1], {20, 5}, &common_allocator, "C"};
			capacity.Draw(screen);
		}
		{
			static CapacityIndicatorUI capacity{p + offsets[2], {20, 5}, &EternalAllocator, "E"};
			capacity.Draw(screen);
		}
		{
			static CapacityIndicatorUI capacity{p + offsets[3], {20, 5}, &builder_allocator, "B"};
			capacity.Draw(screen);
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
