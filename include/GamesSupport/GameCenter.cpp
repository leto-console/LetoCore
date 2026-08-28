#include "GameCenter.hpp"

#include <AppLoader/AppLoader.hpp>
#include <SceneManager/ISceneManager.hpp>

#include <Input/SystemInputID.hpp>
#include <DrawFunctions/DrawText.hpp>
#include <AppLoader/AppScene.hpp>

void GameCenter::RefreshGamesList()
{
	uint32_t count = ScanApps(games_list, games_list_count);

	games_menu.Clear();
	for (uint32_t i = 0; i < count; ++i)
		games_menu.AppendMenuItem(games_list[i].en_name, &games_list[i]);
}

GameCenter::GameCenter(ISceneManager* scene_manager, uint32_t app_scene_id) 
	: IScene{scene_manager}, games_menu{ 7, {0, 8} }, games_list_count{ 16 }, app_scene_id{ app_scene_id }
{
	games_list = static_cast<AppInfo*>( scene_manager->GetCommonAllocator().Alloc(sizeof(AppInfo) * games_list_count) );

	refresh_timer.Start(1000);
	games_menu.InitBaseCatchers();
	games_menu.Enable();
}

void GameCenter::OnShow()
{
	RefreshGamesList();
}

void GameCenter::OnHide()
{
}

void GameCenter::Draw(IScreen& screen)
{
	static StaticText32 title = "------ИГРЫ------";

	DrawFunctions::DrawText(screen, { }, title, WhiteColor, BlackColor);
	games_menu.Draw(screen);
}

void GameCenter::Loop()
{
	if (refresh_timer.Expired())
	{
		refresh_timer.Start();
		RefreshGamesList();
	}

	games_menu.Loop();
}

bool GameCenter::ProcessInput(const AppEvent& event)
{
	if (games_menu.MainProcessInput(event))
		return true;

	if (IsSystemReturnEvent(event))
	{
		scene_manager->Return();
		return true;
	}
	else if (IsSystemEnterEvent(event) && games_menu.Count())
	{
		AppPath = games_menu.GetCurrentParam()->path;
		scene_manager->SwitchScene(app_scene_id);
		return true;
	}

	return false;
}
