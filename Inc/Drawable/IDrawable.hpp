/**
 * @file IDrawable.hpp
 * @date Dec 09, 2025
 * @author Timur
 */

#ifndef INC_DRAWABLE_I_DRAWABLE_HPP_
#define INC_DRAWABLE_I_DRAWABLE_HPP_

#include <Graphics/IScreen.hpp>

#include <stdint.h>

/**
 * @brief Интерфейс для работы с отрисовываемыми объектами
 */
class IDrawable
{
protected:
	/// Координаты левого верхнего угла объекта
	Point2_i position{};

	/// Размер объекта
	Point2_i size{};

	/// Флаг отображения объекта
	bool is_active = true;

public:
	virtual ~IDrawable() = default;

	IDrawable* SetWidth(int width)
	{ 
		size.x = width; 
		return this; 
	}

	int GetWidth() const 
	{ 
		return size.x; 
	}

	IDrawable* SetHeight(int height)
	{
		size.y = height;
		return this; 
	}

	int GetHeight() const
	{
		return size.y;
	}

	// Настройка отображения объекта
	virtual IDrawable* SetActive(bool active = true)
	{
		is_active = active;
		return this;
	}

	virtual bool IsActive() const
	{
		return is_active;
	}

	virtual IDrawable* SetPosition(Point2_i pos)
	{
		this->position = pos;
		return this;
	}

	virtual Point2_i GetPosition() const
	{
		return position;
	}

	virtual void OnShow() { }
	virtual void OnHide() { }

	virtual void Draw(IScreen& screen, Point2_i offset = {}) = 0;
	virtual void Loop() { }

	/**
	 * @brief Главная функция отрисовки объекта
	 * 
	 * Отрисовывается только активный объект (`active` = `false`)
	 */
	void MainDraw(IScreen& screen)
	{
		if (!IsActive()) return;
		Draw(screen);
	}

	/**
	 * @brief Главная функция фоновой обработки объекта
	 * 
	 * Обрабатывается только активный объект (`active` = `true`)
	 */
	void MainLoop()
	{
		if (!IsActive()) return;
		Loop();
	}

};

#endif
