/**
 * @file UI_Align.hpp
 * @date Nov 17, 2026
 * @author Rakhimov T.
 */

#pragma once

#include <cstdint>

#include <UI/Base/UI_Object.hpp>

NEW_LETO_UI_BEGIN

enum class UI_XAlign : uint8_t
{
	LEFT,		// Выравнивание по левому краю
	CENTER,		// Выравнивание по центру
    RIGHT,      // Выравнивание по правому краю
};

enum class UI_YAlign : uint8_t
{
	TOP,		// Выравнивание по верхнему краю
	CENTER,		// Выравнивание по середине
    BOTTOM,     // Выравнивание по нижнему краю
};

class UI_Alignable : public UI_Object
{
public:
	UI_Alignable(Point2_i size = {}, const LetoAllocator_V1* allocator = nullptr) 
		: UI_Object{ size, allocator }
	{ }
	UI_Alignable(Point2_i size, UI_Object& parent)
		: UI_Object{ size, parent }
	{ }

	// Установить горизонтальное выравнивание
	void SetAlign(UI_XAlign _x_align, UI_YAlign _y_align)
	{
		if (x_align != _x_align ||
			y_align != _y_align)
		{
			x_align = _x_align;
			y_align = _y_align;
			if (RefreshAlign()) 
				Invalidate();
		}
	}

	// Установить горизонтальное выравнивание
	void SetXAlign(UI_XAlign align)
	{
		if (x_align != align)
		{
			x_align = align;
			if (RefreshAlign()) 
				Invalidate();
		}
	}

	UI_XAlign GetXAlign() const
	{
		return x_align;
	}

	// Установить вертикальное выравнивание
	void SetYAlign(UI_YAlign align)
	{
		if (y_align != align)
		{
			y_align = align;
			if (RefreshAlign()) 
				Invalidate();
		}
	}

	UI_YAlign GetYAlign() const 
	{ 
		return y_align; 
	}

protected:

	/**
	 * @return Необходимость инвалидировать картинку на экране
	 */
	virtual bool RefreshAlign() = 0;

	bool ObjectOnEvent(const AppEvent& event) override
	{
		if (IsUIEvent(event, UIE_ON_MOVE) ||
			IsUIEvent(event, UIE_ON_RESIZE))
		{
			if (RefreshAlign()) 
				Invalidate();
		}

		return UI_Object::ObjectOnEvent(event);
	}

private:
	UI_XAlign x_align;
	UI_YAlign y_align;
};

NEW_LETO_UI_END
