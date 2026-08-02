/*
 * ISelectable.hpp
 *
 *  Created on: Dec 17, 2025
 *      Author: Timur
 */

#ifndef INC_SCENE_MANAGER_I_SELECTABLE_HPP_
#define INC_SCENE_MANAGER_I_SELECTABLE_HPP_

#include "LibrariesExport.h"

// Класс для элементов, которые могут быть выбраны (например, из списка подряд идущих строк)
class LETO_CORE_EXPORT ISelectable
{
protected:
	// Выделен ли объект
	bool selected{ false };

public:
	virtual ~ISelectable() = default;

	virtual bool IsSelected() const { return selected; }
	virtual void Select(bool select = true) { selected = select; }
};

#endif