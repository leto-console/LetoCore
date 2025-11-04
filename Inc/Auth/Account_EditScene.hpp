/*
 * Account_EditScene.hpp
 *
 *  Created on: Feb 26, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_ACCOUNT_EDIT_SCENE_HPP_
#define INC_AUTH_ACCOUNT_EDIT_SCENE_HPP_

#include "LibrariesExport.h"

#include <Auth/Account.hpp>
#include <Auth/AccountSettingsCells.hpp>
#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>
#include <UI/TextSettingUI.hpp>
#include <UI/SettingsContainer.hpp>

#include <Data/DataCell.hpp>
#include <Data/StaticText.hpp>

class LIBRARIES_EXPORT Account_EditScene : public IScene
{
protected:
	AccountSettingsCells edit_account;
	SettingsContainer settings;

	void OnApply();
	void OnCancel();

public:
	Account_EditScene();

	void OnShow() override;
	bool ProcessInput(const AppEvent& event) override;

	SCENE_NO_ARGS_BUILDER(Account_EditScene)
};

#endif
