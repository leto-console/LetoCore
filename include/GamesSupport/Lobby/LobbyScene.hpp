/**
 * @file LobbyScene.hpp
 * @date Jun 26, 2026
 * @author Rakhimov T. 
 */

#ifndef INC_GAMES_SUPPORT_LOBBY_LOBBY_SCENE_HPP_
#define INC_GAMES_SUPPORT_LOBBY_LOBBY_SCENE_HPP_

#include "LibrariesExport.h"

#include <GamesSupport/Lobby/HostScene.hpp>
#include <GamesSupport/Lobby/MemberScene.hpp>

#include <UI/Menu/ParamMenu.hpp>
#include <UI/Text/UI_Label.hpp>

#include <SceneManager/IScene.hpp>

class LETO_CORE_EXPORT LobbyScene : public IScene
{
public:
    enum LobbyMode { NONE, SELECT, HOST, MEMBER };

    void SwitchMode(LobbyMode mode);

public:
    LobbyScene(ISceneManager* game, uint8_t max_count, 
        LobbyConnection_V1_Callback host_callback,
        LobbyConnection_V1_Callback member_callback);

    void OnShow() override;

    bool ProcessInput(const AppEvent& event) override;

    void Draw(IScreen& screen) override;

    void Loop() override;

    class Builder : public ISceneBuilder
	{
	public:
		Builder(ISceneManager* scene_manager, uint8_t max_count, 
            LobbyConnection_V1_Callback host_callback,
            LobbyConnection_V1_Callback member_callback) 
			: ISceneBuilder{ scene_manager }, 
            max_count{ max_count },
            host_callback{ host_callback }, 
            member_callback{ member_callback } { }
	protected:
		uint8_t max_count;
        LobbyConnection_V1_Callback host_callback;
        LobbyConnection_V1_Callback member_callback;

		IScene* Create(IAllocator& allocator) override
		{
			return allocator.Make<LobbyScene>(scene_manager, max_count, host_callback, member_callback);
		}
	};

protected:
    /// Выбор режима сцены
    LobbyMode scene_mode;

    HostScene host_scene;
    MemberScene member_scene;

    ParamMenu<LobbyMode, 2> select_menu;
    UI_Label select_label;

    uint32_t successID{};
};

#endif
