/*
 * BaseGame.hpp
 *
 *  Created on: Dec 06, 2025
 *      Author: Timur
 */

#ifndef INC_GAMES_SUPPORT_BASE_GAME_SCENE_HPP_
#define INC_GAMES_SUPPORT_BASE_GAME_SCENE_HPP_

#include "LibrariesExport.h"

#include <Graphics/IScreen.hpp>

#include <LetoABI/AppEvent.h>

#include <string>

class BaseGame;

// Интерфейс базовой игровой сцены
class LETO_CORE_EXPORT BaseGameScene
{
protected:
	BaseGame* game{ nullptr };

public:
	BaseGameScene(BaseGame* game);
	virtual ~BaseGameScene() = default;

	// Пользовательский ввод в игру
	virtual void ProcessGameInput(const AppEvent& event) = 0;

	virtual void OnShow() {};

	virtual void OnHide() {};

	// Игровая отрисовка
	virtual void Draw(IScreen& screen) = 0;

	// Фоновая обработка данных
	virtual void Loop() {};
};

#endif
