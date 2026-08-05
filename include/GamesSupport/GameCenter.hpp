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
#include <GamesSupport/GameInfo.hpp>

// Сцены выбора игры + обработки текущей запущенной игры
class LETO_CORE_EXPORT GameCenter : public IScene
{
protected:
	GameInfo* games_list;
	const uint32_t games_list_count;

	ParamMenu<GameInfo*, 16> games_menu;

	/// Проверить, что событие пользовательского ввода нужно отправлять в игру
	bool IsInputForGame(const AppEvent& event);

	Timer refresh_timer;
	void RefreshGamesList();

public:
	GameCenter(ISceneManager* scene_manager);
	~GameCenter();

	void OnShow() override;
	void OnHide() override;

	void Draw(IScreen& screen) override;
	bool Loop() override;
	bool ProcessInput(const AppEvent& event) override;

	SCENE_NO_ARGS_BUILDER(GameCenter)
};

// Системный игровой центр
extern LETO_CORE_EXPORT GameCenter* SystemGameCenter;

#endif
