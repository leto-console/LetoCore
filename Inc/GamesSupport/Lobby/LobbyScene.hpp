/**
 * @file LobbyScene.hpp
 * @date Jun 26, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_LOBBY_SCENE_HPP_
#define INC_GAMES_SUPPORT_LOBBY_LOBBY_SCENE_HPP_

#include "LibrariesExport.h"

#include <GamesSupport/BaseGameScene.hpp>
#include <GamesSupport/Lobby/HostScene.hpp>
#include <GamesSupport/Lobby/MemberScene.hpp>

#include <UI/Menu/ParamMenu.hpp>
#include <UI/Text/UI_Label.hpp>

class LIBRARIES_EXPORT LobbyScene : public BaseGameScene
{
public:
    enum LobbyMode { NONE, SELECT, HOST, MEMBER };

    void SwitchMode(LobbyMode mode);

public:
    LobbyScene(BaseGame* game, uint8_t max_count, 
        LobbyConnection_V1_Callback host_callback,
        LobbyConnection_V1_Callback member_callback);

    void OnShow() override;

    void ProcessGameInput(const AppEvent& event) override;

    void Draw(IScreen& screen) override;

    void Loop() override;

protected:
    /// Выбор режима сцены
    LobbyMode scene_mode;

    HostScene host_scene;
    MemberScene member_scene;

    ParamMenu<LobbyMode, 2> select_menu;
    UI_Label select_label;

    uint32_t prevID{}, successID{};
};

#endif
