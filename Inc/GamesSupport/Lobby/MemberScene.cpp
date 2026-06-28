#include "MemberScene.hpp"

#include <LetoAPI_V1/LetoAPI_V1.hpp>

MemberScene::MemberScene(BaseGame *game, LobbyScene *lobby) : BaseGameScene{ game }
{
    IFont* font = leto_api_v1->Font->GetFont(7, 7, 1);

    label_text.SetText("УЧАСТНИКИ!");
    label_text.SetFont(font);
    label_text.SetActive();
}

void MemberScene::ProcessGameInput(const AppEvent &event)
{
}

void MemberScene::Draw(IScreen &screen)
{
    label_text.MainDraw(screen);
}

void MemberScene::Loop()
{
    label_text.MainLoop();
}
