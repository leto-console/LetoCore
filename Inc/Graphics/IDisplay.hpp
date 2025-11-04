/**
 * @file IDisplay.hpp
 * @date Apr 21, 2026
 * @author Rakhimov T.
 */

#ifndef INC_GRAPHICS_IDISPLAY_HPP_
#define INC_GRAPHICS_IDISPLAY_HPP_

#include <stdint.h>

#include <Graphics/IScreen.hpp>
#include <Graphics/IDrawer.hpp>

/**
 * @brief Интерфейс дисплея как устройства
 */
struct IDisplay
{
	virtual ~IDisplay() = default;

    /// Ширина дисплея в пикселях
	virtual int Width() const = 0;

    /// Высота дисплея в пикселях
	virtual int Height() const = 0;

	/// Дисплей функционирует корректно
	virtual bool IsOpen() = 0;

    /**
     * @brief Проинициализировать дисплей
     */
    virtual bool Init() = 0;

	/**
     * @brief Отправить изображение на дисплей
     * 
     * Отправка может быть разбита на несколько итераций
     * 
     * @param drawer Экземпляр отрисовщика
     * @param method Метод отрисовки
     * 
     * @return Передача изображения окончена
     */
	virtual bool Render(IDrawer* drawer) = 0;

    /**
     * @brief Выполнить действия, напрямую не связанные с отправкой изображения
     */
	virtual void Loop() = 0;
};

#endif
