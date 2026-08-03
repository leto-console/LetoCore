/**
 * @file ISceneObject.hpp
 * @date Mar 30, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SCENE_MANAGER_I_SCENE_OBJECT_HPP_
#define INC_SCENE_MANAGER_I_SCENE_OBJECT_HPP_

#include "LibrariesExport.h"

#include <Drawable/IDrawable.hpp>
#include <SceneManager/ICapturable.hpp>

/**
 * @brief Интерфейс объекта на сцене
 */
struct ISceneObject : public IDrawable, public ICapturable
{
    void Enable(bool value)
    {
        SetActive(value);
		Capture(value);
    }

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
