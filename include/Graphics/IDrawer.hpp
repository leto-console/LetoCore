/**
 * @file IDrawer.hpp
 * @date Apr 22, 2026
 * @author Rakhimov T.
 */

#ifndef INC_GRAPHICS_IDRAWER_HPP_
#define INC_GRAPHICS_IDRAWER_HPP_

#include <stdint.h>

#include <Graphics/IScreen.hpp>

/**
 * @brief Интерфейс для отрисовщика
 */
struct IDrawer
{
	virtual ~IDrawer() = default;
    
	/**
     * @brief Отрисовать изображение на дисплей
     * 
     * Отправка может быть разбита на несколько итераций
     * 
     * @param screen Экземпляр отрисовщика
     * 
     * @warning Отрисовка одного кадра может осуществляться несколькими областями
     */
	virtual void Draw(IScreen& screen) = 0;

    /**
     * @brief Выполнить действия, напрямую не связанные с отрисовкой изображения
     */
	virtual void Loop() = 0;
};

#endif
