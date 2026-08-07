/**
 * @file HostScene.hpp
 * @date Jun 26, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_HOST_SCENE_HPP_
#define INC_GAMES_SUPPORT_LOBBY_HOST_SCENE_HPP_

#include "LibrariesExport.h"

#include <UI/Geometry/UI_Circle.hpp>
#include <UI/Text/UI_Label.hpp>
#include <UI/Menu/ParamMenu.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>

class LobbyScene;

class LETO_CORE_EXPORT HostScene : public IScene
{
public:
    HostScene(ISceneManager* game, LobbyScene* lobby, uint8_t max_count, LobbyConnection_V1_Callback callback);

    void OnShow() override;

    bool ProcessInput(const AppEvent& event) override;

    void Draw(IScreen& screen) override;

    void Loop() override;

protected:
    UI_Label label_text, status_text;

    enum { MEMBERS_COUNT = 6 };
    UI_Label members_text[MEMBERS_COUNT];
    UI_Circle members_ready[MEMBERS_COUNT];

    LobbyScene* main_scene;
    LobbyConnection_V1 lobby;
    bool opened;

    enum { NONE, MENU_START, MENU_QUIT };
    ParamMenu<uint8_t, 8> menu;

    LobbyConnection_V1_Callback callback;

    uint8_t max_count;
    int32_t y_menu_bottom_pos;

    void RefreshMembers();
    void RefreshLobby();
    void RefreshMenu();
    void Quit();
};

#endif
