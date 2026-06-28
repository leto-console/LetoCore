#include "HostScene.hpp"

#include <LetoAPI_V1/LetoAPI_V1.hpp>

HostScene::HostScene(BaseGame *game, LobbyScene *lobby) : BaseGameScene{ game }
{
    IFont* font = leto_api_v1->Font->GetFont(7, 7, 1);

    label_text.SetText("ХОСТ!");
    label_text.SetFont(font);
    label_text.SetActive();
}

void HostScene::ProcessGameInput(const AppEvent &event)
{
}

void HostScene::Draw(IScreen &screen)
{
    label_text.MainDraw(screen);
}

void HostScene::Loop()
{
    label_text.MainLoop();
}
