/*
 * Account_DeleteScene.hpp
 *
 *  Created on: Mar 01, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_ACCOUNT_DELETE_SCENE_HPP_
#define INC_AUTH_ACCOUNT_DELETE_SCENE_HPP_

#include "LibrariesExport.h"

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>
#include <UI/Menu/DialogMenu.hpp>

class LIBRARIES_EXPORT Account_DeleteScene : public IScene
{
protected:
	DialogParamMenu<bool, 4> delete_question;

public:
	Account_DeleteScene();

	bool ProcessInput(const AppEvent& event) override;
	void Draw(IScreen& screen) override;
	bool Loop() override;
	void OnShow() override;

	SCENE_NO_ARGS_BUILDER(Account_DeleteScene)
};

#endif
