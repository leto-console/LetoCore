/**
 * @file HostScene.hpp
 * @date Jun 26, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_HOST_SCENE_HPP_
#define INC_GAMES_SUPPORT_LOBBY_HOST_SCENE_HPP_

#include "LibrariesExport.h"

#include <GamesSupport/BaseGameScene.hpp>

#include <UI/Text/Label.hpp>

class LobbyScene;

class LIBRARIES_EXPORT HostScene : public BaseGameScene
{
public:
    HostScene(BaseGame* game, LobbyScene* lobby);

    void ProcessGameInput(const AppEvent& event) override;

    void Draw(IScreen& screen) override;

    void Loop() override;

protected:
    Label label_text;

};

#endif
