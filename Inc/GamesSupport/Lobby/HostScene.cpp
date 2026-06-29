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
    
    int y_pos = 22;
    status_text = label_text;
    status_text.SetTextColor(GrayColor);
    status_text.SetPosition({0, y_pos});
    status_text.SetText("ОЖИДАНИЕ");
    y_pos += 18;

    for (uint8_t i = 0; i < MEMBERS_COUNT; ++i)
    {
        int32_t x_ready = 10;

        members_ready[i].SetRadius(2);
        members_ready[i].SetMainColor(GreenColor);
        members_ready[i].SetPosition({x_ready, y_pos + 3});
        members_ready[i].Disable();

        members_text[i] = label_text;
        members_text[i].SetPosition({x_ready + 6, y_pos});
        members_text[i].SetHorizonAlignment(LabelHorizonAlignment::LEFT);
        members_text[i].Disable();

        y_pos += 8;
    }
    y_pos += 8;
    y_menu_bottom_pos = y_pos;

    menu.InitBaseCatchers();
    menu.EnableReadyLogic();
    menu.SetStyle(MenuStyle::STYLE_3, font);
    menu.SetHorizonAlignment(MenuHorizonAlignment::CENTER);
    menu.SetPosition({120, 55});
    menu.Enable();

    // In the end because label_text is copy source for other labels
    label_text.SetSize({160, 10});
    label_text.UpdateOffsets();
    label_text.SetTextColor(BlackColor);
    label_text.SetBackroundColor(YellowColor);
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
        Quit();
        return;
    }

    if (menu.ProcessInput(event)) return;
}

void HostScene::Draw(IScreen &screen)
{
    label_text.MainDraw(screen);
    status_text.MainDraw(screen);
    menu.MainDraw(screen);
    for (UI_Label& label : members_text)
        label.MainDraw(screen);
    for (UI_Circle& circle : members_ready)
        circle.MainDraw(screen);

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
    
    RefreshLobby();
        
    if (opened) 
    {
        status_text.SetText("УЧАСТНИКИ:");
        RefreshMembers();
    }
    else
    {
        status_text.SetText("ЗАКРЫТО");
    }

    RefreshMenu();
}

void HostScene::RefreshMembers()
{
    for (UI_Label& label : members_text)
        label.Disable();

    for (UI_Circle& circle : members_ready)
        circle.Disable();
    
    if (!opened) return;

    for (int i = 0; i < lobby.connected; ++i)
    {                
        WebDeviceInfo_V1 info;
        if (leto_api_v1->Web->FindDeviceNear(lobby.members[i], &info))
        {
            members_text[i].SetText(info.web_name);
            members_text[i].SetTextColor(info.id == leto_api_v1->Globals->GetDeviceID() ? label_text.GetBackroundColor() : WhiteColor);
            members_text[i].Enable();
            if (info.flags & WD_FLAG_READY)
                members_ready[i].Enable();
        }
    }
}

void HostScene::RefreshLobby()
{
    opened = leto_api_v1->Lobby->GetActiveLobby(&lobby);
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
