/*
 * AuthScene.hpp
 *
 *  Created on: Feb 21, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_AUTH_SCENE_HPP_
#define INC_AUTH_AUTH_SCENE_HPP_

#include "LibrariesExport.h"

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>
#include <UI/Menu/ParamMenu.hpp>
#include <UI/Text/UI_Label.hpp>
#include <Auth/Account_CreateScene.hpp>

class LETO_CORE_EXPORT AuthScene : public IScene
{
protected:
	IScene* current_subscene{};
	Account_CreateScene create_scene;

	UI_Label label_input;
	ParamMenu<int, 16> menu;

public:
	AuthScene();

	// Отображать сцену с выбором аккаунтов
	void ShowSelectScene();

	// Отображать сцену с созданием аккаунта
	void ShowCreateScene();

	void OnShow() override;

	void Draw(IScreen& screen) override;
	bool ProcessInput(const AppEvent& event) override;
	bool Loop() override;

	SCENE_NO_ARGS_BUILDER(AuthScene)
};

#endif
