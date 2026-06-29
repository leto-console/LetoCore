/**
 * @file MemberScene.hpp
 * @date Jun 26, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_MEMBER_SCENE_HPP_
#define INC_GAMES_SUPPORT_LOBBY_MEMBER_SCENE_HPP_

#include "LibrariesExport.h"

#include <GamesSupport/BaseGameScene.hpp>
#include <UI/Text/UI_Label.hpp>
#include <UI/Menu/ParamMenu.hpp>
#include <LetoAPI_V1/LetoAPI_V1.hpp>

class LobbyScene;

class LIBRARIES_EXPORT MemberScene : public BaseGameScene
{
public:
    MemberScene(BaseGame* game, LobbyScene* main_scene, LobbyConnection_V1_Callback callback);

    void OnShow() override;

    void ProcessGameInput(const AppEvent& event) override;

    void Draw(IScreen& screen) override;

    void Loop() override;

protected:
    UI_Label label_text, status_text;

    LobbyScene* main_scene;
    LobbyConnection_V1 lobby;
    bool opened;

    LobbyInfo_V1 near_info[8]{};
    uint32_t near_cnt{};

    enum { NONE, MENU_START, MENU_QUIT, HOST_ID = 10 };
    ParamMenu<uint8_t, 8> menu;

    LobbyConnection_V1_Callback callback;
    
    void RefreshLobbiesNear();
    void RefreshMenu();
    void Quit();
};

#endif
