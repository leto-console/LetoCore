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
	class LIBRARIES_EXPORT Circle : public IDrawable
	{
	protected:
		int radius{}, outline{};

	public:
		// Радиус и размер контура (при outline = 0 круг заливается)
		Circle(int radius, int outline = 0);

		void SetRadius(int radius);
		int GetRadius() const;

		void SetOutline(int outline);
		int GetOutline() const;

		void Draw(IScreen& screen, Point2_i offset = {}) override;
	};
}

#endif