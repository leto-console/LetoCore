// /*
//  * PrepareWebGameScene.hpp
//  *
//  *  Created on: Feb 22, 2025
//  *      Author: Timur
//  */

// #ifndef INC_GAMES_GAME_CENTER_HPP_
// #define INC_GAMES_GAME_CENTER_HPP_

// #include "LibrariesExport.h"

// #include <SceneManager/IScene.hpp>

// #include <UI/Menu/ParamMenu.hpp>
// #include <Time/Timer.hpp>
// #include <GamesSupport/BaseGame.hpp>
// #include <Data/StaticText.hpp>
// #include <Graphics/IFont.hpp>

// #include <vector>

// #include <LetoAPI_V1/Web/WebConnection.hpp>

//  // Сцена, предназначенная для подготовки к сетевой игре
// class LIBRARIES_EXPORT PrepareWebGameScene : public BaseGameScene
// {
// protected:
// 	WebConnection_V1 connection;
// 	bool inited{};

// 	// Ячейка с именем соперника
// 	StaticText32& opp_name;

// 	// Ячейка с аватаркой соперника
// 	uint32_t& opp_avatar;

// 	// Ячейка с номером канала соперника
// 	uint8_t& opp_channel;

// 	// Ячейка с признаком сетевой игры
// 	bool* ext_web_mode{};

// 	// Ячейка с информацией о том, кто пригласил
// 	bool* ext_my_move{};

// 	Timer wait_timer;
// 	Timer refresh_timer;

// 	IFont* font;

// 	struct GamerListInfo
// 	{
// 		uint8_t channel;
// 		uint32_t avatar;
// 	};

// 	ParamMenu<GamerListInfo, 8> gamers_list;
// 	ParamMenu<bool, 4> request_menu;

// 	uint32_t prevID{}, successID{};

// 	enum WebState
// 	{
// 		SELECT,				// Выбор игрока
// 		REQUEST,			// Получен запрос. Подготовка ответа
// 		WAIT_RESPONSE,		// Сделан запрос. Ожидается ответ
// 		TIMEOUT_RESPONSE,	// Ответ не получен за выделенное время
// 		REJECT_RESPONSE,	// Отказ на запрос
// 	};
// 	WebState web_state = SELECT;

// 	void RefreshGamersList();

// public:
// 	PrepareWebGameScene(BaseGame* game, 
// 		StaticText32* opp_name, uint32_t* opp_avatar, uint8_t* opp_channel,
// 		uint32_t successID);
// 	~PrepareWebGameScene();

// 	void SetParams(bool* ext_web_mode = nullptr, bool* ext_my_move = nullptr);

// 	void OnShow() override;

// 	// Пользовательский ввод в игру
// 	void ProcessGameInput(const AppEvent& event) override;

// 	// Игровая отрисовка
// 	void Draw(IScreen& screen) override;

// 	void Loop() override;
// };
// #endif
