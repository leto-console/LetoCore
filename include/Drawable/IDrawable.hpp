/**
 * @file IDrawable.hpp
 * @date Dec 09, 2025
 * @author Rakhimov T.
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

	/// TODO: Внедрить поддержку "грязных" прямоугольников
	/// Флаг валидности отображаемого объекта (необходимо для рендеринга)
	bool is_valid = false;

public:
	virtual ~IDrawable() = default;

	IDrawable& SetSize(Point2_i size)
	{
		Invalidate();
		this->size = size;
		Invalidate();
		return *this;
	}

	IDrawable& SetWidth(int width)
	{ 
		Invalidate();
		size.x = width;
		Invalidate();
		return *this; 
	}

	int GetWidth() const 
	{ 
		return size.x; 
	}

	IDrawable& SetHeight(int height)
	{
		Invalidate();
		size.y = height;
		Invalidate();
		return *this; 
	}

	int GetHeight() const
	{
		return size.y;
	}

	// Настройка отображения объекта
	virtual IDrawable& SetActive(bool active = true)
	{
		is_active = active;
		Invalidate();
		return *this;
	}

	virtual bool IsActive() const
	{
		return is_active;
	}

	virtual void Validate()
	{
		is_valid = true;
	}

	virtual void Invalidate()
	{
		is_valid = false;
		/// TODO: Добавить логику отправки областей, которые нужно отрендерить
	}

	virtual bool IsValid() const
	{
		return is_valid;
	}

	virtual IDrawable& SetPosition(Point2_i pos)
	{
		Invalidate();
		this->position = pos;
		Invalidate();
		return *this;
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
