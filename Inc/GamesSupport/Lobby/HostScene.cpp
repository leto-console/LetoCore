#include "HostScene.hpp"

#include <LetoAPI_V1/LetoAPI_V1.hpp>

#include <GamesSupport/BaseGame.hpp>
#include <GamesSupport/Lobby/LobbyScene.hpp>

HostScene::HostScene(BaseGame* game, LobbyScene* main_scene, uint8_t max_count, LobbyConnection_V1_Callback callback) 
    : BaseGameScene{ game }, main_scene{ main_scene }, max_count{ max_count }, callback{ callback }
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
    status_text.SetText("ОЖИДАНИЕ");
    y_pos += 10;

    for (Label& label : members_text)
    {
        label = label_text;
        label.SetPosition({0, y_pos});
        label.Disable();
        y_pos += 8;
    }
    y_pos += 8;

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

void HostScene::OnShow()
{
    opened = leto_api_v1->Lobby->CreateLobby(&lobby, game->GetID(), max_count, callback);
    menu.ResetCurrentID();
}

void HostScene::ProcessGameInput(const AppEvent &event)
{
    if (!opened)
    {
        main_scene->SwitchMode(LobbyScene::SELECT);
        return;
    }

    if (menu.ProcessInput(event)) return;
}

void HostScene::Draw(IScreen &screen)
{
    label_text.MainDraw(screen);
    status_text.MainDraw(screen);
    menu.MainDraw(screen);
    for (Label& label : members_text)
        label.MainDraw(screen);

    if (opened)
    {
        
    }
}

void HostScene::Loop()
{
    label_text.MainLoop();
    status_text.MainLoop();
    menu.MainLoop();

    uint8_t menu_param;
    if (menu.IsResultParamReady(menu_param))
    {
        menu.SubmitReady();
        if (menu_param == MENU_QUIT)
        {
            Quit();
        }
        else if (menu_param == MENU_START)
        {
            //.... Logic of start the game
        }
        return;
    }

    for (Label& label : members_text)
        label.Disable();
    
    opened = leto_api_v1->Lobby->GetActiveLobby(&lobby);
    if (opened) 
    {
        if (lobby.connected == 1)
            status_text.SetText("ОЖИДАНИЕ");
        else
        {
            status_text.SetText("УЧАСТНИКИ:");
            for (int i = 0; i < lobby.connected; ++i)
            {                
                WebDeviceInfo_V1 info;
                if (leto_api_v1->Web->FindDeviceNear(lobby.members[i], &info))
                {
                    members_text[i].SetText(info.web_name);
                    members_text[i].SetActive();
                }
            }
        }
    }
    else
    {
        status_text.SetText("ЗАКРЫТО");
    }

    RefreshMenu();
}

void HostScene::RefreshMenu()
{
    menu.Clear();

    if (opened && lobby.connected > 1)
        menu.AppendMenuItem("НАЧАТЬ", MENU_START);
    menu.AppendMenuItem("ВЫЙТИ", MENU_QUIT);    
}

void HostScene::Quit()
{
    leto_api_v1->Lobby->QuitLobby();
    main_scene->SwitchMode(LobbyScene::SELECT);
}
