// #include "PrepareWebGameScene.hpp"

// #include <cstring>

// #include <LetoAPI_V1/LetoAPI_V1.h>

// void PrepareWebGameScene::RefreshGamersList()
// {
// 	if (!refresh_timer.Expired())
// 		return;

// 	gamers_list.Clear();

// 	WebDeviceInfo_V1 dev_info[8]{};
// 	uint32_t count = leto_api_v1->Web->GetDevicesNear(dev_info, sizeof(dev_info) / sizeof(WebDeviceInfo_V1));

// 	for (uint32_t i = 0; i < count; ++i)
// 	{
// 		if (dev_info[i].app_id && dev_info[i].app_id == game->GetID())
// 		{
// 			StaticText32 name;
// 			memcpy(name.CharPtr(), dev_info[i].web_name, 32);
// 			gamers_list.AppendMenuItem(name, { dev_info[i].channel, dev_info[i].avatar_id });
// 		}
// 	}

// 	refresh_timer.Start();
// }

// struct PrepareGameData
// {
// 	uint16_t GameID;
// 	uint8_t command;	
// };


// static void PrepareWebGameScene_Callback(uint8_t channel, uint8_t port, uint32_t id, const void* data, uint32_t size)
// {
// 	//// Запрос начала игры
// 	//if (data[2] == 0x01)
// 	//{
// 	//	for (uint8_t i = 0; i <= gamers_list.Count(); ++i)
// 	//	{
// 	//		if (gamers_list.GetParam(i).channel == msg_in.channel)
// 	//		{
// 	//			opp_name = gamers_list.GetText(i);
// 	//			opp_avatar = gamers_list.GetParam(i).avatar;
// 	//			opp_channel = msg_in.channel;
// 	//			web_state = REQUEST;
// //
// 	//			if (ext_my_move) *ext_my_move = false;
// 	//		
// 	//			break;
// 	//		}
// 	//	}
// 	//	return;
// 	//}
// //
// 	//if (web_state == WAIT_RESPONSE)
// 	//{
// 	//	// Ответ: подтверждение
// 	//	if (msg_in.data[3] == 0x01)
// 	//	{
// 	//		if (ext_my_move) *ext_my_move = true;
// 	//		if (ext_web_mode) *ext_web_mode = true;
// //
// 	//		scene_manager->SwitchScene(successID);
// 	//	}
// 	//	// Ответ: отказ
// 	//	else if (msg_in.data[3] == 0x02)
// 	//	{
// 	//		if (ext_web_mode) *ext_web_mode = false;
// //
// 	//		web_state = REJECT_RESPONSE;
// 	//	}
// 	//}
// }

// PrepareWebGameScene::PrepareWebGameScene(ISceneManager* game, StaticText32* opp_name,  uint32_t* opp_avatar, uint8_t* opp_channel, uint32_t successID)
// 	: IScene{ game }, gamers_list{ 4 }, request_menu{ 2 },
// 	opp_name{ *opp_name }, opp_avatar{ *opp_avatar }, opp_channel{ *opp_channel }, successID { successID }
// {
// 	leto_api_v1->Support->Import("");

// 	inited = leto_api_v1->Web->CreateConnection(&connection, WC_CHANNEL_ALL, WC_PORT_APP, 0, &PrepareWebGameScene_Callback);

// 	font = leto_api_v1->Font->GetFont(7, 7, 1);

// 	refresh_timer.Start(1000);
// 	wait_timer.Start(15000);
// 	gamers_list.SetStyle(MenuStyle::STYLE_2, font);

// 	request_menu.SetStyle(MenuStyle::STYLE_3, font);
// 	request_menu.SetHorizonAlignment(MenuHorizonAlignment::CENTER);
// 	request_menu.AppendMenuItem("ДА", true);
// 	request_menu.AppendMenuItem("НЕТ", false);
// }

// PrepareWebGameScene::~PrepareWebGameScene()
// {
// 	leto_api_v1->Web->CloseConnection(connection);
// }

// void PrepareWebGameScene::SetParams(bool* ext_web_mode, bool* ext_my_move)
// {
// 	this->ext_web_mode = ext_web_mode;
// 	this->ext_my_move = ext_my_move;
// }

// void PrepareWebGameScene::OnShow()
// {
// 	if (!inited) game->Close();

// 	prevID = game->GetGameSceneID();
// 	web_state = SELECT;
// }


// #include <Input/ButtonEvent.hpp>
// #include <GamesSupport/GameInputID.hpp>
// #include <DrawFunctions/DrawBitmap.hpp>
// #include <DrawFunctions/DrawText.hpp>

// void PrepareWebGameScene::ProcessInput(const AppEvent& event)
// {
// 	if (web_state == WAIT_RESPONSE)
// 		return;
	
// 	if (ButtonEvent::IsPressed(event))
// 	{
// 		if (web_state == TIMEOUT_RESPONSE || web_state == REJECT_RESPONSE)
// 		{
// 			web_state = SELECT;
// 			return;
// 		}

// 		if (ButtonEvent::Compare(event, GameLeftEvent()))
// 		{
// 			if (web_state == SELECT)
// 			{
// 				scene_manager->SwitchScene(prevID);
// 			}
// 		}
// 		else if (ButtonEvent::Compare(event, GameRightEvent()) || 
// 				ButtonEvent::Compare(event, GameEnterEvent()))
// 		{
// 			if (web_state == SELECT)
// 			{
// 				if (gamers_list.Count() == 0)
// 					return;

// 				opp_name = gamers_list.GetCurrentText();
// 				opp_avatar = gamers_list.GetCurrentParam().avatar;
// 				opp_channel = gamers_list.GetCurrentParam().channel;

// 				PrepareGameData data;
// 				data.GameID = game->GetID();
// 				data.command = 0x01;

// 				leto_api_v1->Web->SendData(connection, )

// 				//WebMessage msg{};
// //
// 				//// Канал для подключения
// 				//msg.channel = opp_channel;
// //
// 				//// Запрос начала игры
// 				//msg.data[2] = 0x01;

// 				//game->WebAckOut(msg);

// 				wait_timer.Start();
// 				web_state = WAIT_RESPONSE;
// 			}
// 			else if (web_state == REQUEST)
// 			{
// 				//WebMessage msg{};
// //
// 				//// Канал для подключения
// 				//msg.channel = opp_channel;
// //
// 				//// ДА
// 				//if (request_menu.GetCurrentParam())
// 				//{
// 				//	if (ext_web_mode) *ext_web_mode = true;
// //
// 				//	// Подтверждение
// 				//	msg.data[3] = 0x01;
// 				//	scene_manager->SwitchScene(successID);
// 				//}
// 				// НЕТ
// 				//else
// 				//{
// 				//	if (ext_web_mode) *ext_web_mode = false;
// //
// 				//	// Отказ
// 				//	msg.data[3] = 0x02;
// 				//	scene_manager->SwitchScene(prevID);
// 				//}
// 				//game->WebAckOut(msg);
// 			}
// 		}
// 		else if (ButtonEvent::Compare(event, GameUpEvent()))
// 		{
// 			if (web_state == SELECT)
// 				gamers_list.Up();
// 			else if (web_state == REQUEST)
// 				request_menu.Up();
// 		}
// 		else if (ButtonEvent::Compare(event, GameDownEvent()))
// 		{
// 			if (web_state == SELECT)
// 				gamers_list.Down();
// 			else if (web_state == REQUEST)
// 				request_menu.Down();
// 		}
// 	}
// }

// void PrepareWebGameScene::Draw(IScreen& screen)
// {
// 	using namespace DrawFunctions;

// 	StaticText32 text;

// 	if (web_state == SELECT)
// 	{
// 		text = "В СЕТИ";
// 		DrawText(screen, { 64 - TextWidth(text) / 2, 0 }, text);

// 		gamers_list.Draw(screen, {40, 20});

// 		if (gamers_list.Count() == 0)
// 		{
// 			text = "НИКОГО...";
// 			DrawText(screen, {64 - TextWidth(text) / 2, 20}, text, WhiteColor, BlackColor, false, font);
// 		}
// 		else
// 		{
// 			BitmapData avatar;
// 			if (leto_api_v1->User->GetAvatarByID(gamers_list.GetCurrentParam().avatar, &avatar))
// 			{
// 				DrawFunctions::DrawBitmap(screen, {10, 15}, avatar, WhiteColor);
// 			}

// 		}
// 	}
// 	else if (web_state == WAIT_RESPONSE)
// 	{
// 		text = "ЖДЕМ...";

// 		if (wait_timer.Expired())
// 		{
// 			web_state = TIMEOUT_RESPONSE;
// 			return;
// 		}
// 		// TODO: выделить все опции нужные для отрисовки в отдельный объект, который можно будет использовать для разных подряд идущих функций (передавая при этом его по ссылке)
// 		DrawText(screen, {64 - TextWidth(text) / 2, 20}, text, WhiteColor, BlackColor, false, font);
// 	}
// 	else if (web_state == TIMEOUT_RESPONSE)
// 	{
// 		text = "НЕУДАЧА";
// 		DrawText(screen, {64 - TextWidth(text) / 2, 20}, text, WhiteColor, BlackColor, false, font);
// 	}
// 	else if (web_state == REJECT_RESPONSE)
// 	{
// 		text = "ОТКАЗАНО";
// 		DrawText(screen, {64 - TextWidth(text) / 2, 20}, text, WhiteColor, BlackColor, false, font);
// 	}
// 	else if (web_state == REQUEST)
// 	{
// 		text = "НАЧАТЬ ИГРУ";
// 		DrawText(screen, {64 - TextWidth(text) / 2, 0}, text, WhiteColor, BlackColor, false, font);

// 		text = opp_name;
// 		DrawText(screen, {45, 20}, text, WhiteColor, BlackColor, true, font);

// 		BitmapData avatar;
// 		if (leto_api_v1->User->GetAvatarByID(opp_avatar, &avatar))
// 		{
// 			DrawFunctions::DrawBitmap(screen, {20, 15}, avatar, WhiteColor);
// 		}

// 		request_menu.Draw(screen, {64, 40});
// 	}
// }

// void PrepareWebGameScene::Loop()
// {
// 	RefreshGamersList();
// }
