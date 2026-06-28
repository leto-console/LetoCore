#include "MemberScene.hpp"

#include <LetoAPI_V1/LetoAPI_V1.hpp>
#include "LobbyScene.hpp"

#include <Input/SystemInputID.hpp>

MemberScene::MemberScene(BaseGame* game, LobbyScene* main_scene, LobbyConnection_V1_Callback callback) 
    : BaseGameScene{ game }, main_scene{ main_scene }, callback{ callback }
{
    IFont* font = leto_api_v1->Font->GetFont(7, 7, 1);

    label_text.SetText("ИГРОВАЯ КОМНАТА");
    label_text.SetFont(font);
    label_text.SetPosition({0, 10});
    label_text.SetSize({160, 10});
    label_text.SetHorizonAlignment(LabelHorizonAlignment::CENTER);
    label_text.SetActive();

    int y_pos = 30;
    status_text = label_text;
    status_text.SetPosition({0, y_pos});
    status_text.SetText("ХОСТЫ:");
    y_pos += 10;

    menu.InitBaseCatchers();
    menu.EnableReadyLogic();
    menu.SetStyle(MenuStyle::STYLE_3, font);
    menu.SetHorizonAlignment(MenuHorizonAlignment::CENTER);
    menu.SetPosition({80, y_pos});
    menu.Enable();

    // In the end because label_text is copy source for other labels
    label_text.SetTextColor(BlackColor);
    label_text.SetBackroundColor(CyanColor);
}

void MemberScene::OnShow()
{
    menu.ResetCurrentID();
}

void MemberScene::ProcessGameInput(const AppEvent &event)
{
    if (!opened && IsSystemLeftEvent(event))
    {
        Quit();
        return;
    }

    menu.ProcessInput(event);
}

void MemberScene::Draw(IScreen &screen)
{
    label_text.MainDraw(screen);
    status_text.MainDraw(screen);
    menu.MainDraw(screen);
}

void MemberScene::Loop()
{
    label_text.MainLoop();
    status_text.MainLoop();
    menu.MainLoop();

    uint8_t param;
    if (menu.IsResultParamReady(param))
    {
        menu.SubmitReady();
        if (param >= HOST_ID)
        {
            uint32_t host = param - HOST_ID;
            leto_api_v1->Lobby->JoinLobby(near_info[host], callback);
        }
        else if (param == MENU_QUIT)
        {
            Quit();
        }
        else if (param == MENU_START)
        {
            //.... Logic of start the game
        }
        return;
    }

    RefreshLobbiesNear();

    if (opened)
    {
        status_text.SetText("ПОДКЛЮЧЕНЫ!");
    }
    else if (near_cnt)
    {
        status_text.SetText("ХОСТЫ:");
    }
    else
    {
        status_text.SetText("ОЖИДАЕМ");
    }

    RefreshMenu();
}

void MemberScene::RefreshLobbiesNear()
{
    opened = leto_api_v1->Lobby->GetActiveLobby(&lobby);
    near_cnt = leto_api_v1->Lobby->GetLobbiesNear(near_info, sizeof(near_info) / sizeof(near_info[0]));
}

void MemberScene::RefreshMenu()
{
    menu.Clear();

    if (!opened)
    {
        for (uint32_t i = 0; i < near_cnt; ++i)
        {
            char txt[16];
            leto_api_v1->Text->FormatText(txt, sizeof(txt), "%d", near_info[i].owner);
            menu.AppendMenuItem(txt, HOST_ID + i);
        }
    }
    else
    {
        menu.AppendMenuItem("НАЧАТЬ", MENU_START);
        menu.AppendMenuItem("ВЫЙТИ", MENU_QUIT);    
    }
}

void MemberScene::Quit()
{
    leto_api_v1->Lobby->QuitLobby();
    main_scene->SwitchMode(LobbyScene::SELECT);
}
