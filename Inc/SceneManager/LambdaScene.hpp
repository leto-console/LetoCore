/*
 * LambdaScene.hpp
 *
 *  Created on: Oct 19, 2025
 *      Author: Timur
 */

#ifndef INC_SCENE_MANAGER_LAMBDA_SCENE_HPP_
#define INC_SCENE_MANAGER_LAMBDA_SCENE_HPP_

#include <SceneManager/IScene.hpp>

#include <Graphics/IScreen.hpp>

 // Интерфейс экранной сцены, поведение которой задается с помощью лябмды
class LIBRARIES_EXPORT LambdaScene : public IScene
{
public:
	using LambdaSceneDrawCallback = void(*)(IScreen&);

protected:
	LambdaSceneDrawCallback callback;

public:
	LambdaScene(LambdaSceneDrawCallback callback) 
		: callback{ callback } 
	{ 
	}
	virtual ~LambdaScene() = default;

	void Draw(IScreen& screen) override 
	{ 
		callback(screen); 
	};
	bool Loop() override 
	{ 
		return true;
	};
	bool ProcessInput(const AppEvent& event) override 
	{ 
		return false; 
	};
};

#endif