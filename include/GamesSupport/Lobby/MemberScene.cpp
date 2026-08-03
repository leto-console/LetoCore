#include "MemberScene.hpp"

#include <LetoAPI_V1/LetoAPI_V1.h>
#include "LobbyScene.hpp"

#include <Input/SystemInputID.hpp>

static const StaticText32 ST_AVAILABLE_HOSTS{ "ДОСТУПНЫЕ ХОСТЫ:" }; ///< ДОСТУПНЫЕ ХОСТЫ:
static const StaticText32 ST_CONNECTED{ "ВЫ ПОДКЛЮЧЕНЫ!" };         ///< ВЫ ПОДКЛЮЧЕНЫ
static const StaticText32 ST_WAITING{ "ОЖИДАЕМ" };                  ///< ОЖИДАЕМ

MemberScene::MemberScene(BaseGame* game, LobbyScene* main_scene, uint8_t max_count, LobbyConnection_V1_Callback callback) 
    : HostScene{ game, main_scene, max_count, callback }
{
    status_text.SetText(ST_AVAILABLE_HOSTS);
    label_text.SetBackroundColor(DeepOrangeColor);
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

    if (menu.ProcessInput(event)) return;
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
            leto_api_v1->Lobby->SetReady(true);
        }
        return;
    }

    for (UI_Label& label : members_text)
        label.Disable();

    for (UI_Circle& circle : members_ready)
        circle.Disable();

    RefreshLobby();

    if (opened)
    {
        status_text.SetText(ST_CONNECTED);
        RefreshMembers();
    }
    else if (near_cnt)
    {
        status_text.SetText(ST_AVAILABLE_HOSTS);
    }
    else
    {
        status_text.SetText(ST_WAITING);
    }

    RefreshMenu();
}

void MemberScene::RefreshLobby()
{
    HostScene::RefreshLobby();
    near_cnt = leto_api_v1->Lobby->GetLobbiesNear(near_info, sizeof(near_info) / sizeof(near_info[0]));
}

void MemberScene::RefreshMenu()
{
    menu.Clear();

    if (!opened)
    {
        menu.SetPosition({80, 40});
        for (uint32_t i = 0; i < near_cnt; ++i)
        {
            char txt[16];
            leto_api_v1->Text->FormatText(txt, sizeof(txt), "%d", near_info[i].owner);
            menu.AppendMenuItem(txt, HOST_ID + i);
        }
    }
    else
    {
        menu.SetPosition({120, 55});
        if (!leto_api_v1->Lobby->GetReady())
            menu.AppendMenuItem("НАЧАТЬ", MENU_START);
        menu.AppendMenuItem("ВЫЙТИ", MENU_QUIT);    
    }
}
