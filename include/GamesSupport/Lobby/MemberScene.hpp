/**
 * @file MemberScene.hpp
 * @date Jun 26, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_MEMBER_SCENE_HPP_
#define INC_GAMES_SUPPORT_LOBBY_MEMBER_SCENE_HPP_

#include "LibrariesExport.h"

#include <GamesSupport/Lobby/HostScene.hpp>
#include <UI/Geometry/UI_Circle.hpp>
#include <UI/Text/UI_Label.hpp>
#include <UI/Menu/ParamMenu.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

#include <SceneManager/ISceneManager.hpp>

class LobbyScene;

class LETO_CORE_EXPORT MemberScene : public HostScene
{
public:
    MemberScene(ISceneManager* game, LobbyScene* main_scene, uint8_t max_count, LobbyConnection_V1_Callback callback);

    void OnShow() override;

    bool ProcessInput(const AppEvent& event) override;

    // void Draw(IScreen& screen) override;

    void Loop() override;

protected:

    enum { NONE, MENU_START, MENU_QUIT, HOST_ID = 10 };

    LobbyInfo_V1 near_info[8]{};
    uint32_t near_cnt{};

    void RefreshLobby();
    void RefreshMenu();
    //void Quit();
};

#endif
