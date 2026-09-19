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
#include <System/DeviceID_Scene.hpp>

class LETO_CORE_EXPORT AuthScene : public IScene
{
protected:
	IScene* current_subscene{};
	Account_CreateScene create_scene;
	DeviceID_Scene device_id_scene;

	UI_Label label_input;
	ParamMenu<int, 16> menu;

	void ShowDeviceID_Scene();

public:
	AuthScene(ISceneManager* scene_manager, IDataCell<uint32_t>* SerialNumber);

	// Отображать сцену с выбором аккаунтов
	void ShowSelectScene();

	// Отображать сцену с созданием аккаунта
	void ShowCreateScene();

	void OnShow() override;

	void Draw(IScreen& screen) override;
	bool ProcessInput(const AppEvent& event) override;
	void Loop() override;

	SCENE_ONE_ARG_BUILDER(AuthScene, IDataCell<uint32_t>*)
};

#endif
