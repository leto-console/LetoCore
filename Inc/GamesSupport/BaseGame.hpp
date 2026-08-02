/*
 * BaseGame.hpp
 *
 *  Created on: Dec 06, 2025
 *      Author: Timur
 */

#ifndef INC_GAMES_BASE_GAME_HPP_
#define INC_GAMES_BASE_GAME_HPP_

#include "LibrariesExport.h"

#include <Graphics/IScreen.hpp>
#include <Utils/crc16.hpp>

#include <GamesSupport/BaseGameScene.hpp>

#include <Data/RingFIFO_Static.hpp>
#include <Data/IAllocator.hpp>
#include <Data/StaticText.hpp>

#include <LetoABI/AppBinHeader.h>
#include <LetoABI/AppEvent.h>

#include <LetoAPI_V1/LetoAPI_V1.h>

 // Интерфейс базовой игры
class LETO_CORE_EXPORT BaseGame
{
protected:
	// Текущая игровая сцена
	enum { GAME_SCENES = 32 };
	BaseGameScene* game_scenes[GAME_SCENES]{};
	uint32_t current_scene_id{};

	/// Заголовок игры с общей информацией
	AppBinHeader header;

	// Флаг, что игра закрыта
	bool close_flag = false;

	BaseGameScene* CurrentScene() const;

	// Добавление игровых сцен (вызывается в Init)
	BaseGameScene* AddGameScene(uint32_t ID, BaseGameScene* scene);

	template <typename ID>
	BaseGameScene* AddGameScene(ID id, BaseGameScene* scene)
	{
		return AddGameScene((uint32_t)id, scene);
	}

public:
	BaseGame(const AppBinHeader& header);
	virtual ~BaseGame();

	uint32_t GetGameSceneID() const { return current_scene_id; };
	uint16_t GetID() const { return header.id; }

	template <typename GameScene, typename ID, typename... Args>
	GameScene* AddGameScene(ID id, Args... arg)
	{
		static_assert(std::is_base_of<BaseGameScene, GameScene>::value);
		IAllocator* allocator = leto_api_v1->Globals->GetAllocator();
		GameScene* scene = allocator->Make<GameScene>(arg...);
		AddGameScene((uint32_t)id, scene);
		return scene;
	}

	// Очистка игровых сцен (вызывается в Close)
	void ClearGameScenes();

	// Переключение на сцену номер 0 означает выход из игры
	void SwitchGameScene(uint32_t ID);

	// Признак завершения игры
	bool IsClosed() const { return close_flag; };

	// Инициализация игры
	virtual bool Init() = 0;

	// Функция завершения игры
	virtual void Close() = 0;

	// Пользовательский ввод в игру
	virtual void ProcessGameInput(const AppEvent& event);

	// Игровая отрисовка
	virtual void Draw(IScreen& screen);

	// Фоновая обработка данных
	virtual void Loop();
};

#endif
