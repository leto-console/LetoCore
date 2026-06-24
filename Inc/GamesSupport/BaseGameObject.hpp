/**
 * @file BaseGameObject.hpp
 * @date Apr 06, 2026
 * @author Rakhimov T.
 */

#ifndef INC_GAME_SUPPORT_BASE_GAME_OBJECT_HPP_
#define INC_GAME_SUPPORT_BASE_GAME_OBJECT_HPP_

#include <Drawable/IDrawable.hpp>
#include <SceneManager/ICapturable.hpp>

/**
 * @brief Интерфейс объекта на игровой сцене
 */
struct BaseGameObject : public IDrawable, public ICapturable
{
    void Enable()
    {
        SetActive(true);
		Capture(true);
    }

    void Disable()
    {
        SetActive(false);
		Capture(false);
    }
};

#endif
