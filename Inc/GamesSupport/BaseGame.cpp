#include <GamesSupport/BaseGame.hpp>

#include <cstring>

/// TODO: Выделить механизм обмена инфой между приложениями

//bool BaseGame::IsMyWebMessage(const WebMessage& msg_in) const
//{
//	// Проверка соответствия ID
//	return (msg_in.data[0] == ((header.id >> 0) & 0xFF) &&
//			msg_in.data[1] == ((header.id >> 8) & 0xFF));
//}
//
//void BaseGame::MakeMyWebMessage(WebMessage& msg_out) const
//{
//	// Тип сообщения - игровое
//	msg_out.port = WebMessageType::WEB_TYPE_GAME;
//
//	// ID запущенной игры
//	msg_out.data[0] = (header.id >> 0) & 0xFF;
//	msg_out.data[1] = (header.id >> 8) & 0xFF;
//}

BaseGameScene* BaseGame::CurrentScene() const
{
	if (current_scene_id >= GAME_SCENES)
		return nullptr;
	return game_scenes[current_scene_id];
}

BaseGame::BaseGame(const AppBinHeader& header) : header{ header }
{
}

BaseGame::~BaseGame()
{
	ClearGameScenes();
}

BaseGameScene* BaseGame::AddGameScene(uint32_t ID, BaseGameScene* scene)
{
	if (ID >= GAME_SCENES)
		return nullptr;
	game_scenes[ID] = scene;
	return scene;
}

void BaseGame::ClearGameScenes()
{
	for (uint32_t id = 0; id < GAME_SCENES; ++id)
	{
		if (game_scenes[id]) game_scenes[id]->~BaseGameScene();
	}
	current_scene_id = 0;
	memset(game_scenes, 0, sizeof(game_scenes));
}

void BaseGame::SwitchGameScene(uint32_t ID)
{
	if (ID >= GAME_SCENES)
		return;
	if (game_scenes[ID])
	{
		if (CurrentScene())
			CurrentScene()->OnHide();
		game_scenes[ID]->OnShow();
		current_scene_id = ID;
	}
	if (ID == 0)
		Close();
}

void BaseGame::ProcessGameInput(const AppEvent& event)
{
	if (CurrentScene())
		CurrentScene()->ProcessGameInput(event);
}

void BaseGame::Draw(IScreen& screen)
{
	if (CurrentScene())
		CurrentScene()->Draw(screen);
}

void BaseGame::Loop()
{
	if (CurrentScene())
		CurrentScene()->Loop();
}

//void BaseGame::WebIn(const WebMessage& msg_in)
//{
//	// Проверка соответствия КС
//	if (!IsMyWebMessage(msg_in))
//		return;
//
//	CommonWebIn(msg_in);
//
//	if (CurrentScene())
//		CurrentScene()->WebIn(msg_in);
//}
//
//// Обработка получения подтверждений игровых синхро-сообщений
//void BaseGame::WebAckIn(const WebMessage& msg_in)
//{
//	// Проверка соответствия КС
//	if (!IsMyWebMessage(msg_in))
//		return;
//
//	if (CurrentScene())
//		CurrentScene()->WebAckIn(msg_in);
//}
//
//// Выдача игровых синхро-сообщений
//void BaseGame::WebOut(WebMessage& msg_out)
//{
//	MakeMyWebMessage(msg_out);
//
//	web_out.Push(msg_out);
//}
//
//// Выдача игровых синхро-сообщений с подтверждением
//void BaseGame::WebAckOut(WebMessage& msg_out)
//{
//	MakeMyWebMessage(msg_out);
//
//	// Сообщение с подтверждением
//	msg_out.port |= WEB_EXTRA_NEED_ACK;
//
//	web_out.Push(msg_out);
//}
//
//bool BaseGame::CheckWebOut(WebMessage& msg_out)
//{
//	return web_out.Front(msg_out);
//}
//
//void BaseGame::NextWebOut()
//{
//	web_out.Pop();
//}
