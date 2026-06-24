/**
 * @file CapacityIndicatorUI
 * @date Mar 26, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_CAPACITY_INDICATOR_UI_HPP_
#define INC_UI_CAPACITY_INDICATOR_UI_HPP_

#include <Drawable/IDrawable.hpp>
#include <Data/ICapacity.hpp>
#include <DrawFunctions/DrawRectangle.hpp>

#include <cmath>

class CapacityIndicatorUI : public virtual IDrawable
{
protected:
	bool horizontal = true;		///< Горизонтальность индикатора

	ICapacity* object;			///< Отображаемый объект

public:
	CapacityIndicatorUI(Point2_i position, Point2_i size, ICapacity* object, bool horizontal = true)
		: object{ object }, horizontal{ horizontal }
	{
		SetPosition(position);
		SetWidth(size.x);
		SetHeight(size.y);
	}

	/* Обновляет текущее поле */
	void Loop() override
	{
		//cur_idx = -1;
		//T get_value = cell->GetOrDefault();
		//for (int i = 0; i < (int)list.size(); i++)
		//{
		//	if (list[i].value == get_value)
		//	{
		//		cur_idx = i;
		//		break;
		//	}
		//}
	}

	void Draw(IScreen& screen, Point2_i offset = {}) override
	{
		using namespace DrawFunctions;

		float perc = object->GetPercentage();

		Point2_i lu = position + offset;
		Point2_i rd = lu + size;
	
		DrawOutlinedRectangle(screen, lu, rd, BlackColor, WhiteColor);

		lu += {2, 2};
		rd -= {2, 2};

		if (horizontal)
		{
			int width = rd.x - lu.x;
			if (width < 0)
				return;
			
			rd.x = lu.x + static_cast<int>(std::roundf(perc * width));
			if (rd.x == lu.x) return;
		}
		else 
		{
			int height = rd.y - lu.y;
			if (height < 0)
				return;
			
			rd.y = lu.y + static_cast<int>(std::roundf(perc * height));
			if (rd.y == lu.y) return;
		}

		DrawRectangle(screen, lu, rd, WhiteColor);
	}
};

#endif
