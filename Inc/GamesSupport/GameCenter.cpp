#include "GameCenter.hpp"
#include "GameLoader.hpp"

#include <SceneManager/SceneManager.hpp>

#include <Input/SystemInputID.hpp>
#include <DrawFunctions/DrawText.hpp>

#include <LetoAPI_V1_System/Make.hpp>
#include <LetoAPI_V1/Application/LetoApplication.hpp>

GameCenter* SystemGameCenter = nullptr;

bool GameCenter::IsInputForGame(const AppEvent &event)
{
	if (event.source == AE_Button)
	{
		for (uint8_t id : GameButtonIds)
		{
			if (event.id == id) 
				return true;
		}
		return false;
	}
	else if (event.source == AE_Encoder)
	{
		return event.id == GAME_ENC_MAIN;
	}

	return false;
}

void GameCenter::RefreshGamesList()
{
	uint32_t count = ScanGames(games_list, games_list_count);

	games_menu.Clear();
	for (uint32_t i = 0; i < count; ++i)
		games_menu.AppendMenuItem(games_list[i].en_name, &games_list[i]);
}

GameCenter::GameCenter() : games_menu{ 7, {0, 8} }, games_list_count{ 16 }
{
	games_list = static_cast<GameInfo*>( leto_api_v1->Globals->Alloc(sizeof(GameInfo) * games_list_count) );

	refresh_timer.Start(1000);
	SystemGameCenter = this;
	games_menu.Enable();
}

GameCenter::~GameCenter()
{
	SystemGameCenter = nullptr;
}

void GameCenter::OnShow()
{
	RefreshGamesList();
}

void GameCenter::OnHide()
{
	UnloadGame();
}

void GameCenter::Draw(IScreen& screen)
{
	static StaticText32 title = "------ИГРЫ------";

	if (CurrentLoadedGame)
	{
		if (CurrentLoadedGame->api_version == 1)
			reinterpret_cast<LetoApplication_V1*>(CurrentLoadedGame)->Draw(&screen);
	}
	else
	{
		DrawFunctions::DrawText(screen, { }, title, WhiteColor, BlackColor);
		games_menu.Draw(screen);
	}
}

bool GameCenter::Loop()
{
	if (CurrentLoadedGame)
	{
		if (CurrentLoadedGame->api_version == 1)
		{
			LetoApplication_V1* app = reinterpret_cast<LetoApplication_V1*>(CurrentLoadedGame);
			app->Loop();
			if (app->GetStatus() != LETO_V1_WORK_STATUS)
			{
				// ... + некоторые действия при завершении игры
				UnloadGame();
				OnShow();
			}
		}
	}
	else
	{
		if (refresh_timer.Expired())
		{
			refresh_timer.Start();
			RefreshGamesList();
		}

		games_menu.Loop();
	}
	return true;
}

bool GameCenter::ProcessInput(const AppEvent& event)
{
	if (CurrentLoadedGame)
	{
		if (IsInputForGame(event))
		{
			if (CurrentLoadedGame->api_version == 1)
			{
				reinterpret_cast<LetoApplication_V1*>(CurrentLoadedGame)->ProcessEvent(event);
			}
		}
		return true;
	}

	if (games_menu.MainProcessInput(event))
		return true;

	if (IsSystemReturnEvent(event))
	{
		SceneManager::Instance().Return();
		return true;
	}
	else if (IsSystemEnterEvent(event))
	{
		LoadGame(games_menu.GetCurrentParam()->path);
		return true;
	}

	return false;
}
