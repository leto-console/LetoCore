/**
 * @file MemberScene.hpp
 * @date Jun 26, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_MEMBER_SCENE_HPP_
#define INC_GAMES_SUPPORT_LOBBY_MEMBER_SCENE_HPP_

#include "LibrariesExport.h"

#include <GamesSupport/BaseGameScene.hpp>
#include <UI/Text/Label.hpp>

class LobbyScene;

class LIBRARIES_EXPORT MemberScene : public BaseGameScene
{
public:
    MemberScene(BaseGame* game, LobbyScene* lobby);

    void ProcessGameInput(const AppEvent& event) override;

    void Draw(IScreen& screen) override;

    void Loop() override;

protected:
    Label label_text;
};

#endif
