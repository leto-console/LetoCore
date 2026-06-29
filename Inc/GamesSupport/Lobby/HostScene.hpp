/**
 * @file HostScene.hpp
 * @date Jun 26, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_HOST_SCENE_HPP_
#define INC_GAMES_SUPPORT_LOBBY_HOST_SCENE_HPP_

#include "LibrariesExport.h"

#include <GamesSupport/BaseGameScene.hpp>

#include <UI/Geometry/UI_Circle.hpp>
#include <UI/Text/UI_Label.hpp>
#include <UI/Menu/ParamMenu.hpp>
#include <LetoAPI_V1/LetoAPI_V1.hpp>

class LobbyScene;

class LIBRARIES_EXPORT HostScene : public BaseGameScene
{
public:
    HostScene(BaseGame* game, LobbyScene* lobby, uint8_t max_count, LobbyConnection_V1_Callback callback);

    void OnShow() override;

    void ProcessGameInput(const AppEvent& event) override;

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
    ParamMenu<uint8_t, 2> menu;

    uint8_t max_count;

    LobbyConnection_V1_Callback callback;

    void RefreshMenu();
    void Quit();
};

#endif
