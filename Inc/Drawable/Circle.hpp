/*
 * IDrawable.hpp
 *
 *  Created on: Dec 09, 2025
 *      Author: Timur
 */

#ifndef INC_DRAWABLE_CIRCLE_HPP_
#define INC_DRAWABLE_CIRCLE_HPP_

#include "LibrariesExport.h"

#include <Drawable/IDrawable.hpp>

namespace Drawable
{
	class LETO_CORE_EXPORT Circle : public IDrawable
	{
	protected:
		int radius{}, outline{};

		RGBColor color, background;

	public:
		// Радиус и размер контура (при outline = 0 круг заливается)
		explicit Circle(int radius = 0, int outline = 0);

		void SetRadius(int radius);
		int GetRadius() const;

		void SetOutline(int outline);
		int GetOutline() const;

		Circle& SetMainColor(RGBColor color);
		Circle& SetBackroundColor(RGBColor color);

		void Draw(IScreen& screen, Point2_i offset = {}) override;
	};
}

#endif