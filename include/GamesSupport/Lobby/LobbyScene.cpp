#include "LobbyScene.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

#include <GamesSupport/BaseGame.hpp>
#include <Input/SystemInputID.hpp>
#include <System/DeviceID.hpp>

using namespace DrawFunctions;

LobbyScene::LobbyScene(ISceneManager* game, uint8_t max_count, 
    LobbyConnection_V1_Callback host_callback, LobbyConnection_V1_Callback member_callback) 
    : IScene{ game }, scene_mode{ NONE }, 
    host_scene{ game, this, max_count, host_callback }, 
    member_scene{ game, this, max_count, member_callback }
{
    const IFont* font = IFont::FromHandle(leto_api_v1->Font->GetFont(7, 7, 1));

    select_label.SetText("ИГРОВАЯ КОМНАТА");
    select_label.SetPosition({0, 10});
    select_label.SetSize({160, 30});
    select_label.SetHorizonAlignment(LabelHorizonAlignment::CENTER);
    select_label.SetFont(font);
    select_label.Enable();

    select_menu.InitBaseCatchers();
    select_menu.EnableReadyLogic();
    select_menu.SetStyle(MenuStyle::STYLE_3, font);
    select_menu.SetHorizonAlignment(MenuHorizonAlignment::CENTER);
    select_menu.SetPosition({80, 30});
    select_menu.Enable();

    SwitchMode(SELECT);
}

void LobbyScene::OnShow()
{
    LobbyConnection_V1 connection;
    if (leto_api_v1->Lobby->GetActiveLobby(&connection))
    {
        SwitchMode(connection.owner == leto_api_v1->Globals->GetDeviceID() ? HOST : MEMBER);
        return;     
    }
    else
        SwitchMode(SELECT);

    select_menu.Clear();
    select_menu.AppendMenuItem("СОЗДАТЬ", HOST);
    select_menu.AppendMenuItem("НАЙТИ", MEMBER);
}

bool LobbyScene::ProcessInput(const AppEvent &event)
{
    bool _return = IsSystemLeftEvent(event, true);

    switch (scene_mode)
    {
    case HOST:
    {
        host_scene.ProcessInput(event);
        return true;
    }
    case MEMBER:
    {
        member_scene.ProcessInput(event);
        return true;
    }
    case SELECT:
    default:
        break;
    }
    
    if (_return) 
    {
        scene_manager->Return();
        return true;
    }
    select_menu.MainProcessInput(event);

    LobbyMode mode;
    if (select_menu.IsResultParamReady(mode))
    {
        select_menu.SubmitReady();
        SwitchMode(mode);
    }

    return false;
}

void LobbyScene::Draw(IScreen &screen)
{
    switch (scene_mode)
    {
    case SELECT:
    {
        /// TODO: Добавить GameScreenObjects
        select_label.MainDraw(screen);
        select_menu.MainDraw(screen);
        break;
    }
    case HOST:
    {
        host_scene.Draw(screen);
        break;
    }
    case MEMBER:
    {
        member_scene.Draw(screen);
        break;
    }
    default:
        break;
    }
}

bool LobbyScene::Loop()
{
    switch (scene_mode)
    {
    case HOST:
    {
        host_scene.Loop();
        break;
    }
    case MEMBER:
    {
        member_scene.Loop();
        break;
    }
    case SELECT:
    default:
        break;
    }
    return true;
}

void LobbyScene::SwitchMode(LobbyMode mode)
{
    switch (scene_mode)
    {
    case HOST:
    {
        host_scene.OnHide();
        break;
    }
    case MEMBER:
    {
        member_scene.OnHide();
        break;
    }
    case SELECT:
    default:
        break;
    }

    //////////////////////////////////////////////////////////////////////
    scene_mode = mode;
    //////////////////////////////////////////////////////////////////////

    switch (scene_mode)
    {
    case HOST:
    {
        host_scene.OnShow();
        break;
    }
    case MEMBER:
    {
        member_scene.OnShow();
        break;
    }
    case SELECT:
    default:
        break;
    }
}
