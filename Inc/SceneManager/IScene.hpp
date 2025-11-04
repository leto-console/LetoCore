/**
 * @file IScene.hpp
 * @date Oct 19, 2025
 * @author Timur
 */

#ifndef INC_SCENE_MANAGER_I_SCENE_HPP_
#define INC_SCENE_MANAGER_I_SCENE_HPP_

#include "LibrariesExport.h"

#include <Graphics/IScreen.hpp>
#include <ABI/AppEvent.hpp>
#include <Data/StaticList.hpp>
#include <SceneManager/ISceneObject.hpp>
#include <System/CommonAllocator.hpp>

/**
 * @brief Интерфейс экранной сцены
 * @headerfile IScene.hpp <SceneManager/IScene.hpp>
 */
class IScene
{
protected:
	friend class SceneManager;

	/// TODO: Сделать двунаправленный связанный список, чтобы не привязываться к количеству объектов
	/// Объекты экранной сцены
	StaticList<ISceneObject*, 16> objects{};

public:
	virtual ~IScene() = default;

	virtual void Draw(IScreen& screen) { };
	virtual bool Loop() { return true; };
	virtual bool ProcessInput(const AppEvent& event) { return false; };

	void AddObject(ISceneObject* object)
	{
		objects.Push(object);
	}

	template <typename Object, typename... Args>
	Object* AddObject(Args... args)
	{
		static_assert(std::is_base_of<ISceneObject, Object>::value);
		Object* object{};
		if (!objects.Full())
		{
			object = CommonAllocator.Make<Object>(std::forward<Args>(args)...);
			objects.Push(object);
		}
		return object;
	}

	/**
	 * @brief Функция, вызываемая при входе в сцену
	 */
	virtual void OnShow() {	}

	/**
	 * @brief Функция, вызываемая при выходе из сцены
	 */
	virtual void OnHide() {}

	/**
	 * @brief Главная функция, вызываемая при входе в сцену
	 */
	void MainOnShow() 
	{
		OnShow();
		for (ISceneObject*& obj : objects)
			obj->OnShow();
	}

	/**
	 * @brief Главная функция, вызываемая при выходе из сцены
	 */
	void MainOnHide() 
	{
		OnHide();
		for (ISceneObject*& obj : objects)
			obj->OnHide();
	}

	/**
	 * @brief Главная функция отрисовки сцены
	 */
	void MainDraw(IScreen& screen) 
	{
		for (ISceneObject*& obj : objects.reversed())
			obj->MainDraw(screen);
		Draw(screen);
	}

	/**
	 * @brief Главная функция фоновой обработки
	 */
	bool MainLoop()
	{
		for (ISceneObject*& obj : objects)
			obj->MainLoop();
		return Loop();
	}

	/**
	 * @brief Главная функция обработки событий
	 */
	bool MainProcessInput(const AppEvent& event)
	{
		for (ISceneObject*& obj : objects)
		{
			if (obj->MainProcessInput(event)) 
				return true;
		}
		return ProcessInput(event);
	}
};

#endif
