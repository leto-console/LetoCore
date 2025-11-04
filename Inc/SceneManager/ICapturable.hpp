/**
 * @file ICapturable.hpp
 * @date Dec 17, 2025
 * @author Timur
 */

#ifndef INC_SCENE_MANAGER_I_CAPTURABLE_HPP_
#define INC_SCENE_MANAGER_I_CAPTURABLE_HPP_

#include <ABI/AppEvent.hpp>

/**
 * @brief Интерфейс элементов, которые могут захватить поток событий (ввод)
 * @headerfile ICapturable.hpp <SceneManager/ICapturable.hpp>
 */
class ICapturable
{
protected:
	/// Захвачен ли ввод (если захвачен, будет обработано входящее событие)
	bool captured{ false };

public:
	virtual ~ICapturable() = default;

	/// Захвачен ли поток событий
	virtual bool IsCaptured() const { return captured; }

	/// Захватить поток событий
	virtual void Capture(bool capture = true) { captured = capture; }

	/**
	 * @brief Функция для обработки входящих событий
	 */
	virtual bool ProcessInput(const AppEvent& event) = 0;

	/**
	 * @brief Главная функция обработки входящих событий объектом
	 * 
	 * Обрабатывается только захваченный объект (`captured` = `true`)
	 */
	bool MainProcessInput(const AppEvent& event)
	{
		if (!IsCaptured()) return false;
		return ProcessInput(event);
	}
	
};

#endif