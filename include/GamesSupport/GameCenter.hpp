/**
 * @brief GameCenter.hpp
 * @date Dec 06, 2025
 * @author Rakhimov T.
 */

#ifndef INC_GAMES_GAME_CENTER_HPP_
#define INC_GAMES_GAME_CENTER_HPP_

#include "LibrariesExport.h"

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>

#include <UI/Menu/ParamMenu.hpp>
#include <Time/Timer.hpp>
#include <Data/ArenaAllocator.hpp>
#include <Data/StaticList.hpp>

#include <GamesSupport/BaseGame.hpp>
#include <AppLoader/AppInfo.hpp>

// Сцены выбора игры + обработки текущей запущенной игры
class LETO_CORE_EXPORT GameCenter : public IScene
{
protected:
	AppInfo* games_list;
	const uint32_t games_list_count;

	ParamMenu<AppInfo*, 16> games_menu;

	uint32_t app_scene_id;

	Timer refresh_timer;
	void RefreshGamesList();

public:
	GameCenter(ISceneManager* scene_manager, uint32_t app_scene_id);

	void OnShow() override;
	void OnHide() override;

	void Draw(IScreen& screen) override;
	void Loop() override;
	bool ProcessInput(const AppEvent& event) override;

	SCENE_ONE_ARG_BUILDER(GameCenter, uint32_t)
};

#endif
