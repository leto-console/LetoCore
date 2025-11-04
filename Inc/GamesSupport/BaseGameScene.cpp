#include <GamesSupport/BaseGameScene.hpp>
#include <GamesSupport/BaseGame.hpp>


//// Выдача игровых синхро-сообщений
//void BaseGameScene::WebOut(WebMessage& msg_out)
//{
//	if (!game)
//		return;
//	game->WebOut(msg_out);
//}
//
//// Выдача игровых синхро-сообщений
//void BaseGameScene::WebAckOut(WebMessage& msg_out)
//{
//	if (!game)
//		return;
//	game->WebAckOut(msg_out);
//}

BaseGameScene::BaseGameScene(BaseGame* game)
	: game{ game }
{
}

