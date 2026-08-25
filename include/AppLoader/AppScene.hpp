/**
 * @brief AppScene.hpp
 * @date Aug 25, 2025
 * @author Rakhimov T.
 */

#ifndef INC_APP_LOADER_APP_SCENE_HPP_
#define INC_APP_LOADER_APP_SCENE_HPP_

#include "LibrariesExport.h"

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>

#include <GamesSupport/BaseGame.hpp>
#include <AppLoader/AppInfo.hpp>

// Сцены выбора игры + обработки текущей запущенной игры
class LETO_CORE_EXPORT AppScene : public IScene
{
protected:
	/// Проверить, что событие пользовательского ввода нужно отправлять в игру
	bool IsInputForGame(const AppEvent& event);

public:
	AppScene(ISceneManager* scene_manager);

	//void OnShow() override;
	void OnHide() override;

	void Draw(IScreen& screen) override;
	void Loop() override;
	bool ProcessInput(const AppEvent& event) override;

	SCENE_NO_ARGS_BUILDER(AppScene)
};

#endif
