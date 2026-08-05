/*
 * Account_SettingScene.hpp
 *
 *  Created on: Mar 01, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_ACCOUNT_SETTING_SCENE_HPP_
#define INC_AUTH_ACCOUNT_SETTING_SCENE_HPP_

#include "LibrariesExport.h"

#include <Auth/Account.hpp>
#include <Auth/AccountSettingsCells.hpp>
#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>
#include <UI/TextSettingUI.hpp>

#include <Data/DataCell.hpp>
#include <Data/StaticText.hpp>
#include <UI/Menu/ParamMenu.hpp>

class LETO_CORE_EXPORT Account_SettingScene : public IScene
{
protected:
	AccountSettingsCells cur_account;

	ParamMenu<uint32_t, 4> menu;

	// ID сцены с редактированием аккаунта
	uint32_t editID;

	// ID сцены с удалением аккаунта
	uint32_t deleteID;

public:
	Account_SettingScene(ISceneManager* scene_manager, uint32_t editID, uint32_t deleteID);

	void Draw(IScreen& screen) override;
	bool ProcessInput(const AppEvent& event) override;
	void OnShow() override;
	bool Loop() override;

	class Builder : public ISceneBuilder
	{
	public:
		Builder(ISceneManager* scene_manager, uint32_t editID, uint32_t deleteID) 
			: ISceneBuilder{ scene_manager }, editID{ editID }, deleteID{ deleteID } { }
	protected:
		// ID сцены с редактированием аккаунта
		uint32_t editID;
		
		// ID сцены с удалением аккаунта
		uint32_t deleteID;

		IScene* Create(IAllocator& allocator) override
		{
			return allocator.Make<Account_SettingScene>(scene_manager, editID, deleteID);
		}
	};

};



#endif
