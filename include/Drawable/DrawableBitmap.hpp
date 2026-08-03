/*
 * DrawableBitmap.hpp
 *
 *  Created on: Dec 09, 2025
 *      Author: Timur
 */

#ifndef INC_DRAWABLE_CIRCLE_HPP_
#define INC_DRAWABLE_CIRCLE_HPP_

#include "LibrariesExport.h"

#include <Drawable/IDrawable.hpp>
#include <Graphics/BitmapData.hpp>

namespace Drawable
{
	class LETO_CORE_EXPORT DrawableBitmap : public IDrawable
	{
	protected:
		BitmapData& data;
	public:
		// Радиус и размер контура (при outline = 0 круг заливается)
		DrawableBitmap(BitmapData& data);

		void Draw(IScreen& screen, Point2_i offset = {}) override;
	};
}

#endif