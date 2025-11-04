/*
 * Account_CreateScene.hpp
 *
 *  Created on: Feb 26, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_ACCOUNT_CREATE_SCENE_HPP_
#define INC_AUTH_ACCOUNT_CREATE_SCENE_HPP_

#include "LibrariesExport.h"

#include <Auth/Account.hpp>
#include <Auth/AccountSettingsCells.hpp>
#include <SceneManager/IScene.hpp>
#include <UI/TextSettingUI.hpp>
#include <UI/SettingsContainer.hpp>
#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>

class AuthScene;

class LIBRARIES_EXPORT Account_CreateScene : public IScene
{
protected:
	AccountSettingsCells account;
	AuthScene& auth_scene;
	SettingsContainer settings;

	void OnCreateAccount();
	void OnCancel();

public:
	Account_CreateScene(AuthScene& auth_scene);

	void OnShow() override;
};

#endif
